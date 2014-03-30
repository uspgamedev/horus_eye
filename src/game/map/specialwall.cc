#include "game/map/specialwall.h"

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/sprite.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/opengl/vertexdata_rectangle.h>
#include <ugdk/graphic/defaultshaders.h>
#include <ugdk/graphic/texture.h>

#include "game/core/coordinates.h"
#include "game/constants.h"

namespace map {

using namespace ugdk::graphic;
using ugdk::math::Vector2D;
using ugdk::graphic::opengl::VertexBuffer;

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
                                 "uniform highp vec4 effect_color;" "\n");

    fragment_shader.AddCodeBlock("uniform highp vec2 lightUV;" "\n"
                                 "uniform highp vec2 PIXEL_SIZE;" "\n"
                                 "uniform highp sampler2D light_texture;" "\n");
    
    fragment_shader.AddCodeBlock("highp float calculate_offset(highp float x) {" "\n"
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

void SpecialWallDrawFunction(const ugdk::graphic::Primitive& primitive, opengl::ShaderUse& shader_use) {

    auto mgr = ugdk::graphic::manager();

    shader_use.SendUniform("PIXEL_SIZE", 1.0f / mgr->canvas()->size().x, 1.0f / mgr->canvas()->size().y);
    shader_use.SendTexture(1, mgr->light_buffer(), wall_light_shader_->UniformLocation("light_texture"));

    opengl::RenderPrimitiveAsRectangle(primitive, shader_use);
}
}

std::shared_ptr<Primitive> CreateSpecialWall(const ugdk::graphic::Texture* texture) {
    if(!wall_light_shader_) wall_light_shader_ = createWallShader();

    std::shared_ptr<Primitive> primitive(new Primitive(texture, std::make_shared<VertexData>(4, 2 * 2 * sizeof(GLfloat), false)));
    primitive->set_shader_program(wall_light_shader_);
    primitive->set_drawfunction(SpecialWallDrawFunction);

    opengl::PrepareVertexDataAsRectangle(*primitive->vertexdata(), Vector2D(texture->width(), texture->height()));
    ApplyPositionOffset(*primitive->vertexdata(), -wall_hotspot_);

    return primitive;
}

} // namespace map
