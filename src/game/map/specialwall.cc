#include "game/map/specialwall.h"

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/sprite.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/graphic/defaultshaders.h>

#include "game/core/coordinates.h"
#include "game/constants.h"

namespace map {

using namespace ugdk::graphic;
using ugdk::math::Vector2D;

namespace {
Vector2D wall_hotspot_(53, 156);
opengl::ShaderProgram* wall_light_shader_ = nullptr;

opengl::ShaderProgram* createWallShader() {
    opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

    // VERTEX
    vertex_shader.AddCodeBlock("out highp vec2 UV;" "\n");
    vertex_shader.AddCodeBlock("out highp vec4 screenPos;" "\n");
    vertex_shader.AddCodeBlock("uniform highp vec2 lightUV;" "\n");
    vertex_shader.AddLineInMain("	gl_Position = screenPos = geometry_matrix * vec4(vertexPosition,lightUV.y,1);" "\n");
    vertex_shader.AddLineInMain("	UV = vertexUV;" "\n");
    vertex_shader.GenerateSource();

    // FRAGMENT
    fragment_shader.AddCodeBlock("in highp vec2 UV;" "\n"
                                 "in highp vec4 screenPos;" "\n"
                                 "uniform highp sampler2D drawable_texture;" "\n"
                                 "uniform highp vec4 effect_color;" "\n"
                                 "uniform highp vec2 uv_minmax;" "\n"
                                 );

    fragment_shader.AddCodeBlock("uniform highp vec2 lightUV;" "\n"
                                 "uniform highp vec2 PIXEL_SIZE;" "\n"
                                 "uniform highp sampler2D light_texture;" "\n");
    
    fragment_shader.AddCodeBlock("highp float calculate_offset(highp float in_x) {" "\n"
                                 "  highp float x = (in_x - uv_minmax.x) / (uv_minmax.y - uv_minmax.x);"
                                 "  return PIXEL_SIZE.y * 27.0 * min(x, 1.0-x) * 2.0;" "\n" // convert x from 0->0.5->1 to 0->1->0
                                                                                        // 27 is tile_height/2 (54/2)
                                 "}" "\n");

    fragment_shader.AddLineInMain("	highp float screenPosLightX = screenPos.x * 0.5 + 0.5;" "\n");
    fragment_shader.AddLineInMain("	highp vec2 lightPosition = vec2(screenPosLightX, lightUV.y - calculate_offset(UV.x));" "\n");

    fragment_shader.AddLineInMain("	highp vec4 color = texture2D( drawable_texture, UV ) * effect_color;" "\n");
    fragment_shader.AddLineInMain("	color *= vec4(texture2D(light_texture, lightPosition).rgb, 1.0);" "\n");
    fragment_shader.AddLineInMain("	if(color.a <= 0.0) { discard; }" "\n");
    fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
    fragment_shader.GenerateSource();

    opengl::ShaderProgram* shader = new opengl::ShaderProgram;

    shader->AttachShader(vertex_shader);
    shader->AttachShader(fragment_shader);

    bool status = shader->SetupProgram();
    assert(status);

    return shader;
}

void SpecialWallDrawFunction(const Primitive& primitive, opengl::ShaderUse& shader_use) {

    auto mgr = ugdk::graphic::manager();

    shader_use.SendUniform("PIXEL_SIZE", 1.0f / mgr->canvas()->size().x, 1.0f / mgr->canvas()->size().y);
    shader_use.SendTexture(1, mgr->light_buffer(), wall_light_shader_->UniformLocation("light_texture"));

    ugdk::graphic::PrimitiveSetup::Sprite::Render(primitive, shader_use);
}

}

void PreparePrimitiveSpecialWall(ugdk::graphic::Primitive& primitive, const TextureAtlas* atlas, int frame) {
    if(!wall_light_shader_) wall_light_shader_ = createWallShader();

    primitive.set_vertexdata(std::make_shared<VertexData>(4, 2 * 2 * sizeof(GLfloat), false));
    primitive.set_texture(atlas->texture());
    primitive.set_shader_program(wall_light_shader_);

    auto bound_piece = atlas->PieceAt(frame);
    primitive.set_drawfunction([bound_piece](const Primitive& primitive, opengl::ShaderUse& shader_use) {
        float left, right, temp;
        bound_piece.ConvertToAtlas(0.0f, 0.0f, &left, &temp);
        bound_piece.ConvertToAtlas(1.0f, 0.0f, &right, &temp);
        shader_use.SendUniform("uv_minmax", left, right);
        SpecialWallDrawFunction(primitive, shader_use);
    });

    PrimitiveControllerSprite* sprite_controller = new PrimitiveControllerSprite(atlas);
    primitive.set_controller(std::unique_ptr<ugdk::graphic::PrimitiveControllerSprite>(sprite_controller));
    sprite_controller->ChangeToFrame(ugdk::action::SpriteAnimationFrame(frame));
}

} // namespace map

