#include "game/renders/shape.h"

#include <functional>

#include <ugdk/system/engine.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
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

#define PI 3.1415926535897932384626433832795
    
using namespace ugdk;
using namespace ugdk::graphic;
using namespace pyramidworks;
using math::Vector2D;

namespace renders
{

namespace {
    uint16 quad_to_triangles_indices[] = { 0, 1, 2, 0, 2, 3 };
}

void DrawRect(const geometry::Rect* rect, const math::Vector2D& position, ugdk::graphic::Canvas& canvas)
{
    opengl::ShaderProgram::Use shader_use(graphic::manager()->shaders().current_shader());
    shader_use.SendGeometry(canvas.current_geometry());
    shader_use.SendEffect(canvas.current_visualeffect());
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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, quad_to_triangles_indices);
}

void DrawCircle(const geometry::Circle* circle, const Vector2D& position, ugdk::graphic::Canvas& canvas)
{
    opengl::ShaderProgram::Use shader_use(graphic::manager()->shaders().current_shader());
    shader_use.SendGeometry(canvas.current_geometry());
    shader_use.SendEffect(canvas.current_visualeffect());
    shader_use.SendTexture(0, graphic::manager()->white_texture());
    
    math::Vector2D origin = core::FromWorldCoordinates(position);
    
    opengl::VertexArray vertexbuffer(sizeof(GLfloat) * 2 * 10, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Mapper mapper(vertexbuffer);
        GLfloat *vertex_data = static_cast<GLfloat*>(mapper.get());
        vertex_data[0 * 2 + 0] = static_cast<GLfloat>(origin.x);
        vertex_data[0 * 2 + 1] = static_cast<GLfloat>(origin.y);

        for(int i = 0; i < 9; ++i) {
            math::Vector2D p = core::FromWorldCoordinates(
                position + Vector2D(circle->radius(), 0.0).Rotate(i/4.0 * PI));
            vertex_data[(i+1) * 2 + 0] = static_cast<GLfloat>(p.x); // near left
            vertex_data[(i+1) * 2 + 1] = static_cast<GLfloat>(p.y);
        }
    }
    opengl::VertexArray uvbuffer(sizeof(GLfloat) * 2 * 10, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Mapper mapper(uvbuffer);
        GLfloat *uv_data = static_cast<GLfloat*>(mapper.get());
        uv_data[0 * 2 + 0] = uv_data[0 * 2 + 1] = 0.0f;
        for(int i = 1; i < 10; ++i)
            uv_data[i * 2 + 0] = uv_data[i * 2 + 1] = 1.0f;
    }
    shader_use.SendVertexBuffer(&vertexbuffer, opengl::VERTEX, 0);
    shader_use.SendVertexBuffer(&uvbuffer, opengl::TEXTURE, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
}

void DrawShape(const geometry::GeometricShape* shape, const Vector2D& position, ugdk::graphic::Canvas& canvas)
{
    if(auto rect = dynamic_cast<const geometry::Rect*>(shape))
        DrawRect(rect, position, canvas);
    else if(auto circle = dynamic_cast<const geometry::Circle*>(shape))
        DrawCircle(circle, position, canvas);
    else
        assert(false);
}

void DrawCollisionObject(const collision::CollisionObject* collobject, ugdk::graphic::Canvas& canvas) {
    std::size_t hashval = std::hash<std::string>()(collobject->collision_class());
    canvas.PushAndCompose(VisualEffect(Color(static_cast<uint32>(hashval), 0.5)));

    DrawShape(collobject->shape(), collobject->absolute_position(), canvas);

    canvas.PopVisualEffect();
}

}
