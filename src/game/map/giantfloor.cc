#include "game/map/giantfloor.h"

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/defaultshaders.h>

#include "game/core/coordinates.h"

#include "game/map/room.h"
#include "game/scenes/world.h"
#include "game/scenes/lightrendering.h"
#include "game/constants.h"

namespace map {

using namespace ugdk::graphic;
using ugdk::math::Vector2D;
    
namespace {
    ugdk::uint16 quad_to_triangles_indices[] = { 0, 1, 2, 0, 2, 3 };

    ugdk::graphic::opengl::ShaderProgram* continuous_light_shader_ = nullptr;
    void AddHorusLightShader() {
        if (continuous_light_shader_) return;

        opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        // VERTEX
        vertex_shader.AddCodeBlock("out highp vec2 UV;" "\n"
                                   "out highp vec2 lightUV;" "\n"
                                   "uniform highp vec2 ROOM_POSITION;" "\n"
                                   "uniform highp vec2 HOTSPOT;" "\n"
                                   "uniform highp vec2 LEVEL_SIZE;" "\n"
                                   );
        vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1) + vec4(HOTSPOT,0,0);" "\n"
                                    "	UV = vertexUV;" "\n"
                                    "   lightUV = vec2(1, 1) - (ROOM_POSITION + vertexUV.yx + (HOTSPOT + vec2(1,1)) * 0.5) / LEVEL_SIZE;"
                                    );
        vertex_shader.GenerateSource();

        // FRAGMENT
        fragment_shader.AddCodeBlock("in highp vec2 UV;" "\n"
                                     "in highp vec2 lightUV;" "\n"
                                     "uniform highp sampler2D drawable_texture;" "\n"
                                     "uniform highp vec4 effect_color;" "\n"
                                     "uniform highp sampler2D light_texture;" "\n");

        fragment_shader.AddLineInMain("	highp vec4 color = texture2D( drawable_texture, UV ) * effect_color;" "\n");
        fragment_shader.AddLineInMain("	color *= vec4(texture2D(light_texture, lightUV).rgb, 1.0);" "\n");
        fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
        fragment_shader.GenerateSource();


        continuous_light_shader_ = new opengl::ShaderProgram;

        continuous_light_shader_->AttachShader(vertex_shader);
        continuous_light_shader_->AttachShader(fragment_shader);

        bool status = continuous_light_shader_->SetupProgram();
        assert(status);
    }

}

GiantFloor::GiantFloor(const Room* room)
    : room_(room)
    , size_(106 * room->size().x, 54 * room->size().y)
    , texture_(ugdk::resource::GetTextureFromFile("images/ground_texture.png"))
{    
    Vector2D size = room->size();
    AddHorusLightShader();

    GLfloat vertex_data[] = { 
         53.0f,  0.0f, 
        106.0f, 26.0f, 
         53.0f, 52.0f,
          0.0f, 26.0f 
    };
    vertex_data[0 * 2 + 0] = core::FromWorldCoordinates(Vector2D(size.x - 0.5, size.y - 0.5)).x; // top
    vertex_data[0 * 2 + 1] = core::FromWorldCoordinates(Vector2D(size.x - 0.5, size.y - 0.5)).y;
    vertex_data[1 * 2 + 0] = core::FromWorldCoordinates(Vector2D(size.x - 0.5,    0.0 - 0.5)).x; // right
    vertex_data[1 * 2 + 1] = core::FromWorldCoordinates(Vector2D(size.x - 0.5,    0.0 - 0.5)).y;
    vertex_data[2 * 2 + 0] = core::FromWorldCoordinates(Vector2D(   0.0 - 0.5,    0.0 - 0.5)).x; // bottom
    vertex_data[2 * 2 + 1] = core::FromWorldCoordinates(Vector2D(   0.0 - 0.5,    0.0 - 0.5)).y;
    vertex_data[3 * 2 + 0] = core::FromWorldCoordinates(Vector2D(   0.0 - 0.5, size.y - 0.5)).x; // left
    vertex_data[3 * 2 + 1] = core::FromWorldCoordinates(Vector2D(   0.0 - 0.5, size.y - 0.5)).y;
    GLfloat uv_data[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    for(int i = 0; i < 4; ++i) {
        uv_data[i * 2 + 0] *= size.y;
        uv_data[i * 2 + 1] *= size.x;
    }
    vertexbuffer_ = opengl::VertexBuffer::Create(sizeof(vertex_data), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Bind bind(*vertexbuffer_);
        opengl::VertexBuffer::Mapper mapper(*vertexbuffer_);

        GLfloat *indices = static_cast<GLfloat*>(mapper.get());
        if (indices)
            memcpy(indices, vertex_data, sizeof(vertex_data));
    }
    uvbuffer_ = opengl::VertexBuffer::Create(sizeof(uv_data), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Bind bind(*uvbuffer_);
        opengl::VertexBuffer::Mapper mapper(*uvbuffer_);

        GLfloat *indices = static_cast<GLfloat*>(mapper.get());
        if (indices)
            memcpy(indices, uv_data, sizeof(uv_data));
    }
}

GiantFloor::~GiantFloor() {
    delete vertexbuffer_;
    delete uvbuffer_;
}

void GiantFloor::Draw(ugdk::graphic::Canvas& canvas) const {
    if(!room_->level())
        return;

    const glm::mat4& mat = canvas.current_geometry().AsMat4();
    glm::vec4 transformed_hotspot = mat * glm::vec4(-hotspot_.x, -hotspot_.y, 0, 0);

    // Use our shader
    opengl::ShaderUse shader_use(continuous_light_shader_);

    shader_use.SendUniform("HOTSPOT", transformed_hotspot.x, transformed_hotspot.y);
    shader_use.SendUniform("ROOM_POSITION", room_->position().x, room_->position().y);
    shader_use.SendUniform("LEVEL_SIZE", room_->level()->size().x, room_->level()->size().y);

    shader_use.SendTexture(1,
                           room_->level()->light_rendering()->light_texture(),
                           continuous_light_shader_->UniformLocation("light_texture"));

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(mat);
    shader_use.SendEffect(canvas.current_visualeffect());

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, texture_);

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(vertexbuffer_, opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(uvbuffer_, opengl::TEXTURE, 0);

    // Draw the triangle !
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, quad_to_triangles_indices);
}

} // namespace map
