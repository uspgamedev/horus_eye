#include "game/map/specialwall.h"

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/sprite.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/graphic/defaultshaders.h>
#include <ugdk/graphic/rendertarget.h>

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
    vertex_shader.AddCodeBlock("uniform highp float objectDepth;" "\n");
    vertex_shader.AddLineInMain("	gl_Position = geometry_matrix * vec4(vertexPosition,objectDepth,1);" "\n");
    vertex_shader.AddLineInMain("	UV = vertexUV;" "\n");
    vertex_shader.GenerateSource();

    // FRAGMENT
    fragment_shader.AddCodeBlock("in highp vec2 UV;" "\n"
                                 "uniform highp sampler2D drawable_texture;" "\n"
                                 "uniform highp vec4 effect_color;" "\n"

                                 "uniform highp vec2 uv_minmax;" "\n"
                                 "uniform highp vec2 lightUV;" "\n"
                                 "uniform highp vec2 LEVEL_SIZE;" "\n"
                                 "uniform highp sampler2D light_texture;" "\n");
    
    // x = 0.0 -> left corner of the image, maps to bottom-left in-game -> lightUV.x    , lightUV.y + 1
    // x = 0.5 -> bottom-center of the image, maps to top-left in-game  -> lightUV.x    , lightUV.y
    // x = 1.0 -> right corner of the image, maps to top-right in-game  -> lightUV.x + 1, lightUV.y
    fragment_shader.AddLineInMain(
                            "   highp float xPos = (UV.x - uv_minmax.x) / (uv_minmax.y - uv_minmax.x);" "\n"
                            "	highp vec2 lightPosition = vec2("
                            "       lightUV.x + max(0, xPos * 2 - 1) / LEVEL_SIZE.x," "\n"
                            "       lightUV.y + max(0, 1 - xPos * 2) / LEVEL_SIZE.y" "\n"
                            "   );" "\n");

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

void SpecialWallDrawFunction(const Primitive& primitive, Canvas& canvas) {
    ugdk::graphic::PrimitiveSetup::Sprite::Render(primitive, canvas);
}

}

void PreparePrimitiveSpecialWall(ugdk::graphic::Primitive& primitive, const TextureAtlas* atlas, const std::string& frame_name) {
    if(!wall_light_shader_) wall_light_shader_ = createWallShader();

    primitive.set_vertexdata(std::make_shared<VertexData>(4, 2 * 2 * sizeof(GLfloat), false));
    primitive.set_texture(atlas->texture());
    primitive.set_shader_program(wall_light_shader_);

    auto bound_piece = atlas->PieceAt(frame_name);
    primitive.set_drawfunction([bound_piece](const Primitive& primitive, Canvas& canvas) {
        float left, right, temp;
        bound_piece.ConvertToAtlas(0.0f, 0.0f, &left, &temp);
        bound_piece.ConvertToAtlas(1.0f, 0.0f, &right, &temp);
        canvas.SendUniform("uv_minmax", left, right);
        SpecialWallDrawFunction(primitive, canvas);
    });

    PrimitiveControllerSprite* sprite_controller = new PrimitiveControllerSprite(atlas);
    primitive.set_controller(std::unique_ptr<ugdk::graphic::PrimitiveControllerSprite>(sprite_controller));
    sprite_controller->ChangeToAtlasFrame(frame_name);
}

} // namespace map

