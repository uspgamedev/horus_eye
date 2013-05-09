#include <GL/glew.h>
#define NO_SDL_GLEXT

#include "game/map/giantfloor.h"

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/defaultshaders.h>

#include "game/core/coordinates.h"
#include "game/constants.h"

namespace map {

using namespace ugdk::graphic;
using ugdk::math::Vector2D;
    
ugdk::graphic::opengl::ShaderProgram* GiantFloor::continuous_light_shader_ = NULL;

GiantFloor::GiantFloor(const ugdk::math::Integer2D& size)
    : size_(106 * size.x, 54 * size.y),
      texture_(ugdk::base::ResourceManager::GetTextureFromFile("images/ground_texture.png")) {
    
    if(!continuous_light_shader_) {
        continuous_light_shader_ = ugdk::graphic::CreateShader(true, false);
    }

    GLfloat vertex_data[] = { 
         53.0f,  0.0f, 
        106.0f, 26.0f, 
         53.0f, 52.0f,
          0.0f, 26.0f 
    };
    vertex_data[0 * 2 + 0] = core::FromWorldCoordinates(Vector2D(size.x, size.y)).x; // top
    vertex_data[0 * 2 + 1] = core::FromWorldCoordinates(Vector2D(size.x, size.y)).y;
    vertex_data[1 * 2 + 0] = core::FromWorldCoordinates(Vector2D(size.x,    0.0)).x; // right
    vertex_data[1 * 2 + 1] = core::FromWorldCoordinates(Vector2D(size.x,    0.0)).y;
    vertex_data[2 * 2 + 0] = core::FromWorldCoordinates(Vector2D(   0.0,    0.0)).x; // bottom
    vertex_data[2 * 2 + 1] = core::FromWorldCoordinates(Vector2D(   0.0,    0.0)).y;
    vertex_data[3 * 2 + 0] = core::FromWorldCoordinates(Vector2D(   0.0, size.y)).x; // left
    vertex_data[3 * 2 + 1] = core::FromWorldCoordinates(Vector2D(   0.0, size.y)).y;
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

void GiantFloor::Draw(const ugdk::graphic::Geometry& geometry, const ugdk::graphic::VisualEffect& effect) const {
    Geometry final_geometry(geometry);
    final_geometry.Compose(Geometry(Vector2D(-hotspot_)));

    const glm::mat4& mat = final_geometry.AsMat4();
    /*
    glm::vec4 right = mat * glm::vec4(106, 52, 0, 1);
    if(mat[3].x > 1 || mat[3].y < -1 || 
        right.x < -1 || right.y > 1)
        return;*/
    // Use our shader
    opengl::ShaderProgram::Use shader_use(continuous_light_shader_);

    shader_use.SendTexture(1, VIDEO_MANAGER()->light_buffer(), continuous_light_shader_->UniformLocation("light_texture"));

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(mat);
    shader_use.SendEffect(effect);

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, texture_);

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(vertexbuffer_, opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(uvbuffer_, opengl::TEXTURE, 0);

    // Draw the triangle !
    glDrawArrays(GL_QUADS, 0, 4); // 12*3 indices starting at 0 -> 12 triangles
}

} // namespace map
