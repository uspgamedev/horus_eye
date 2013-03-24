#include <GL/glew.h>
#define NO_SDL_GLEXT

#include "game/map/giantfloor.h"

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>

#include "game/core/coordinates.h"
#include "game/constants.h"

namespace map {

using namespace ugdk::graphic;
using ugdk::math::Vector2D;

GiantFloor::GiantFloor(const ugdk::math::Integer2D& size)
    : size_(106, 54),
      texture_(ugdk::base::ResourceManager::GetTextureFromFile("images/ground_texture.png")) {
    
    set_hotspot(Vector2D(constants::GetDouble("FLOOR_HOTSPOT_X"), constants::GetDouble("FLOOR_HOTSPOT_Y")));
    static const GLfloat buffer_data[] = { 
         53.0f,  0.0f, 
        106.0f, 26.0f, 
         53.0f, 52.0f,
          0.0f, 26.0f 
    };
    vertexbuffer_ = opengl::VertexBuffer::Create(sizeof(buffer_data), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Bind bind(*vertexbuffer_);
        opengl::VertexBuffer::Mapper mapper(*vertexbuffer_);

        GLfloat *indices = static_cast<GLfloat*>(mapper.get());
        if (indices)
            memcpy(indices, buffer_data, sizeof(buffer_data));
    }
    uvbuffer_ = opengl::VertexBuffer::CreateDefault();
}

GiantFloor::~GiantFloor() {
    delete vertexbuffer_;
}

void GiantFloor::Draw(const ugdk::graphic::Geometry& geometry, const ugdk::graphic::VisualEffect& effect) const {
    Geometry final_geometry(geometry);
    final_geometry.Compose(Geometry(Vector2D(-hotspot_)));

    const glm::mat4& mat = final_geometry.AsMat4();
    glm::vec4 right = mat * glm::vec4(106, 52, 0, 1);
    if(mat[3].x > 1 || mat[3].y < -1 || 
        right.x < -1 || 
        right.y > 1)
        return;
    // Use our shader
    opengl::ShaderProgram::Use shader_use(VIDEO_MANAGER()->default_shader());

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
