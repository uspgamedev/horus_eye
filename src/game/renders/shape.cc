#include "game/renders/shape.h"

#include <functional>

#include <ugdk/system/engine.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/texture.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionclass.h>
#include <pyramidworks/geometry/rect.h>
#include <pyramidworks/geometry/circle.h>
#include "game/core/coordinates.h"
    
using namespace ugdk;
using namespace ugdk::graphic;
using namespace pyramidworks;

namespace renders
{

void DrawRect(const geometry::Rect* rect, const math::Vector2D& position, const Geometry& geometry, const VisualEffect& effect)
{
    opengl::ShaderProgram::Use shader_use(graphic::manager()->shaders().current_shader());
    shader_use.SendGeometry(geometry);
    shader_use.SendEffect(effect);
    shader_use.SendTexture(0, graphic::manager()->white_texture());

    math::Vector2D top_left     = position + math::Vector2D(rect->width(), rect->height()) * 0.5;
    math::Vector2D bottom_right = position - math::Vector2D(rect->width(), rect->height()) * 0.5;

    math::Vector2D p1s = core::FromWorldCoordinates(math::Vector2D(    top_left.x,     top_left.y)),  // top left
                   p2s = core::FromWorldCoordinates(math::Vector2D(bottom_right.x,     top_left.y)), // bottom left
                   p3s = core::FromWorldCoordinates(math::Vector2D(bottom_right.x, bottom_right.y)), // bottom right
                   p4s = core::FromWorldCoordinates(math::Vector2D(    top_left.x, bottom_right.y)); // top right

    opengl::VertexArray vertexbuffer(sizeof(GLfloat) * 2 * 4, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Mapper mapper(vertexbuffer);
        GLfloat *vertex_data = static_cast<GLfloat*>(mapper.get());
        vertex_data[0 * 2 + 0] = static_cast<GLfloat>(p1s.x); // far left
        vertex_data[0 * 2 + 1] = static_cast<GLfloat>(p1s.y);
        vertex_data[1 * 2 + 0] = static_cast<GLfloat>(p2s.x); // near left
        vertex_data[1 * 2 + 1] = static_cast<GLfloat>(p2s.y);
        vertex_data[2 * 2 + 0] = static_cast<GLfloat>(p3s.x); // near right
        vertex_data[2 * 2 + 1] = static_cast<GLfloat>(p3s.y);
        vertex_data[3 * 2 + 0] = static_cast<GLfloat>(p4s.x); // far right
        vertex_data[3 * 2 + 1] = static_cast<GLfloat>(p4s.y);
    }
    shader_use.SendVertexBuffer(&vertexbuffer, opengl::VERTEX, 0);
    shader_use.SendVertexBuffer(opengl::VertexBuffer::CreateDefault(), opengl::TEXTURE, 0);
    glDrawArrays(GL_QUADS, 0, 4);
}

void DrawCircle(const geometry::Circle* circle, const math::Vector2D& position, const Geometry& geometry, const VisualEffect& effect)
{
}

void DrawShape(const geometry::GeometricShape* shape, const math::Vector2D& position, const Geometry& geometry, const VisualEffect& effect)
{
    if(auto rect = dynamic_cast<const geometry::Rect*>(shape))
        DrawRect(rect, position, geometry, effect);
    else if(auto circle = dynamic_cast<const geometry::Circle*>(shape))
        DrawCircle(circle, position, geometry, effect);
    else
        assert(false);
}

void DrawCollisionObject(const collision::CollisionObject* collobject, const Geometry& geometry, const VisualEffect& effect) {
#ifdef _DEBUG
    Color color(std::hash<std::string>()(collobject->collision_class()->name()), 0.5);
#else
    Color color(std::hash<collision::CollisionClass*>()(collobject->collision_class()), 0.5);
#endif
    DrawShape(collobject->shape(), 
        collobject->absolute_position(), 
        geometry, 
        effect * VisualEffect(color));
}

}