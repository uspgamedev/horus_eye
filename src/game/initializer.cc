#include <sys/stat.h>
#include <cerrno>
#include "initializer.h"

#include <ugdk/system/engine.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/internal/gltexture.h>
   
using namespace ugdk;
using namespace ugdk::graphic;

namespace {
    bool restart_game_ = false;

}

void QueueRestartGame() { 
    restart_game_ = true;
}
bool RestartGameQueued() { 
    return restart_game_;
}

bool VerifyFolderExists(const std::string& path) {
    struct stat st;
    int s;
    if(path[path.size() - 1] == '/')
        s = stat(path.substr(0, path.size() - 1).c_str(), &st);
    else
        s = stat(path.c_str(), &st);
    return !(s < 0 && errno == ENOENT);
}

namespace {

uint16 quad_to_triangles_indices[] = { 0, 1, 2, 0, 2, 3 };

ugdk::graphic::opengl::ShaderProgram* horus_light_shader_ = nullptr;
void AddHorusLightShader() {
    opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);


    // VERTEX
    vertex_shader.AddCodeBlock("out highp vec2 UV;" "\n");
    vertex_shader.AddCodeBlock("uniform highp vec2 lightUV;" "\n");
    vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition,lightUV.y,1);" "\n");
    vertex_shader.AddLineInMain("	UV = vertexUV;" "\n");
    vertex_shader.GenerateSource();

    // FRAGMENT
    fragment_shader.AddCodeBlock("in highp vec2 UV;" "\n"
                                 "uniform highp sampler2D drawable_texture;" "\n"
                                 "uniform highp vec4 effect_color;" "\n");

    fragment_shader.AddCodeBlock("uniform highp vec2 lightUV;" "\n"
                                 "uniform highp sampler2D light_texture;" "\n");

    fragment_shader.AddLineInMain("	highp vec4 color = texture2D( drawable_texture, UV ) * effect_color;" "\n");
    fragment_shader.AddLineInMain("	color *= vec4(texture2D(light_texture, lightUV).rgb, 1.0);" "\n");
    fragment_shader.AddLineInMain("	if(color.a <= 0.0) { discard; }" "\n");
    fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
    fragment_shader.GenerateSource();

    horus_light_shader_ = new opengl::ShaderProgram;

    horus_light_shader_->AttachShader(vertex_shader);
    horus_light_shader_->AttachShader(fragment_shader);

    bool status = horus_light_shader_->SetupProgram();
    assert(status);

    ugdk::graphic::manager()->shaders().ReplaceShader((1 << 0) + (0 << 1), horus_light_shader_);
}

} // namespace

ugdk::graphic::opengl::ShaderProgram* get_horus_light_shader() { return horus_light_shader_; }

void AddHorusShader() {
    AddHorusLightShader();
}

void DrawTexture(ugdk::internal::GLTexture* texture, ugdk::graphic::Canvas& canvas) {
    opengl::ShaderUse shader_use(graphic::manager()->shaders().GetSpecificShader(0));
    shader_use.SendGeometry(canvas.current_geometry());
    shader_use.SendEffect(canvas.current_visualeffect());
    shader_use.SendTexture(0, texture);
    
    opengl::VertexArray vertexbuffer(sizeof(GLfloat) * 2 * 4, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Mapper mapper(vertexbuffer);
        GLfloat *vertex_data = static_cast<GLfloat*>(mapper.get());
        vertex_data[0 * 2 + 0] = 0.0f; // near left
        vertex_data[0 * 2 + 1] = static_cast<GLfloat>(texture->height());
        
        vertex_data[1 * 2 + 0] = 0.0f; // far left
        vertex_data[1 * 2 + 1] = 0.0f;

        vertex_data[2 * 2 + 0] = static_cast<GLfloat>(texture->width()); // near right
        vertex_data[2 * 2 + 1] = static_cast<GLfloat>(texture->height());
        
        vertex_data[3 * 2 + 0] = static_cast<GLfloat>(texture->width()); // far right
        vertex_data[3 * 2 + 1] = 0.0f;

    }
    shader_use.SendVertexBuffer(&vertexbuffer, opengl::VERTEX, 0);
    shader_use.SendVertexBuffer(opengl::VertexBuffer::CreateDefault(), opengl::TEXTURE, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
