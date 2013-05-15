#include <GL/glew.h>
#define NO_SDL_GLEXT

#include "game/map/specialwall.h"

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/defaultshaders.h>
#include <ugdk/graphic/texture.h>

#include "game/core/coordinates.h"
#include "game/constants.h"

namespace map {

using namespace ugdk::graphic;
using ugdk::math::Vector2D;
using ugdk::graphic::opengl::VertexBuffer;
    
ugdk::graphic::opengl::ShaderProgram* SpecialWall::wall_light_shader_ = NULL;

static ugdk::graphic::opengl::ShaderProgram* createWallShader() {
    using namespace ugdk::graphic;

    opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

    // VERTEX
    vertex_shader.AddCodeBlock("out vec2 UV;" "\n");
    vertex_shader.AddCodeBlock("out vec4 screenPos;" "\n");
    vertex_shader.AddLineInMain("	gl_Position = screenPos = geometry_matrix * vec4(vertexPosition,0,1);" "\n");
    vertex_shader.AddLineInMain("	UV = vertexUV;" "\n");
    vertex_shader.GenerateSource();

    // FRAGMENT
    fragment_shader.AddCodeBlock("in vec2 UV;" "\n"
                                 "in vec4 screenPos;" "\n"
                                 "uniform sampler2D drawable_texture;" "\n"
                                 "uniform vec4 effect_color;" "\n");

    fragment_shader.AddCodeBlock("uniform vec2 lightUV;" "\n"
                                 "uniform vec2 PIXEL_SIZE;" "\n"
                                 "uniform sampler2D light_texture;" "\n");
    
    fragment_shader.AddCodeBlock("float calculate_offset(float x) {" "\n"
                                 "  return PIXEL_SIZE.y * 27.0 * min(x, 1-x) * 2;" "\n" // convert x from 0->0.5->1 to 0->1->0
                                                                                        // 27 is tile_height/2 (54/2)
                                 "}" "\n");

    fragment_shader.AddLineInMain("	vec2 screenPosLight = screenPos.xy * 0.5 + vec2(0.5, 0.5);" "\n");
    fragment_shader.AddLineInMain("	vec2 lightPosition = vec2(screenPosLight.x, lightUV.y - calculate_offset(UV.x));" "\n");

    fragment_shader.AddLineInMain("	vec4 color = texture2D( drawable_texture, UV ) * effect_color;" "\n");
    fragment_shader.AddLineInMain("	color *= vec4(texture2D(light_texture, lightPosition).rgb, 1.0);" "\n");
    fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
    fragment_shader.GenerateSource();

    opengl::ShaderProgram* shader = new opengl::ShaderProgram;

    shader->AttachShader(vertex_shader);
    shader->AttachShader(fragment_shader);

    bool status = shader->SetupProgram();
    assert(status);

    return shader;
}

SpecialWall::SpecialWall(const ugdk::graphic::Texture* texture)
    : texture_(texture), size_(texture->width(), texture->height()) {
    
    if(!wall_light_shader_) {
        wall_light_shader_ = createWallShader();
    }
}

SpecialWall::~SpecialWall() {}

void SpecialWall::Draw(const ugdk::graphic::Geometry& geometry, const ugdk::graphic::VisualEffect& effect) const {
    Geometry final_geometry(geometry);
    final_geometry.Compose(Geometry(-hotspot_, size_));

    const glm::mat4& mat = final_geometry.AsMat4();

    // Use our shader
    opengl::ShaderProgram::Use shader_use(wall_light_shader_);

    Vector2D lightpos = geometry.offset() * 0.5 + Vector2D(0.5, 0.5);
    shader_use.SendUniform("lightUV", lightpos.x, lightpos.y);
    shader_use.SendUniform("PIXEL_SIZE", 1/VIDEO_MANAGER()->video_size().x, 1/VIDEO_MANAGER()->video_size().y);

    shader_use.SendTexture(1, VIDEO_MANAGER()->light_buffer(), wall_light_shader_->UniformLocation("light_texture"));

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(mat);
    shader_use.SendEffect(effect);

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, texture_);

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(VertexBuffer::CreateDefault(), opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(VertexBuffer::CreateDefault(), opengl::TEXTURE, 0);

    // Draw the triangle !
    glDrawArrays(GL_QUADS, 0, 4); // 12*3 indices starting at 0 -> 12 triangles
}

} // namespace map
