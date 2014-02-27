#include "game/map/giantfloor.h"

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/defaultshaders.h>

#include "game/core/coordinates.h"
#include "game/constants.h"

namespace map {

using namespace ugdk::graphic;
using ugdk::math::Vector2D;
    
ugdk::graphic::opengl::ShaderProgram* GiantFloor::continuous_light_shader_ = NULL;

namespace {
    ugdk::uint16 quad_to_triangles_indices[] = { 0, 1, 2, 0, 2, 3 };
}

GiantFloor::GiantFloor(const ugdk::math::Integer2D& size)
    : size_(106 * size.x, 54 * size.y),
      texture_(ugdk::resource::GetTextureFromFile("images/ground_texture.png")) {
    
    if(!continuous_light_shader_) {
        continuous_light_shader_ = ugdk::graphic::CreateShader(true, false);
    }

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
    canvas.PushAndCompose(Geometry(-hotspot_));
    const glm::mat4& mat = canvas.current_geometry().AsMat4();
    /*
    glm::vec4 right = mat * glm::vec4(106, 52, 0, 1);
    if(mat[3].x > 1 || mat[3].y < -1 || 
        right.x < -1 || right.y > 1)
        return;*/
    // Use our shader
    opengl::ShaderUse shader_use(continuous_light_shader_);

    shader_use.SendTexture(1, ugdk::graphic::manager()->light_buffer(), continuous_light_shader_->UniformLocation("light_texture"));

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

    canvas.PopGeometry();
}

} // namespace map
