#include "game/renders/shape.h"

#include <functional>

#include <ugdk/system/engine.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
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
    struct VertexXYUV {
        float x, y, u, v;
        void set_xy(const math::Vector2D& v) {
            x = static_cast<float>(v.x);
            y = static_cast<float>(v.y);
        }
    };
}

void DrawRect(const geometry::Rect* rect, const math::Vector2D& position, ugdk::graphic::Canvas& canvas)
{
    TextureUnit unit(graphic::manager()->ReserveTextureUnit(graphic::manager()->white_texture()));
    canvas.SendUniform("drawable_texture", unit);

    math::Vector2D top_left     = position + math::Vector2D(rect->width(), rect->height()) * 0.5;
    math::Vector2D bottom_right = position - math::Vector2D(rect->width(), rect->height()) * 0.5;

    math::Vector2D ps[] = {
        core::FromWorldCoordinates(math::Vector2D(top_left.x, top_left.y)), // top left
        core::FromWorldCoordinates(math::Vector2D(top_left.x, bottom_right.y)), // top right
        core::FromWorldCoordinates(math::Vector2D(bottom_right.x, top_left.y)), // bottom left
        core::FromWorldCoordinates(math::Vector2D(bottom_right.x, bottom_right.y)), // bottom right
    };
    std::tuple<float, float> uv[] = {
        std::make_tuple(0.0f, 0.0f),
        std::make_tuple(1.0f, 0.0f),
        std::make_tuple(0.0f, 1.0f),
        std::make_tuple(1.0f, 1.0f)
    };

    VertexData data(4, sizeof(VertexXYUV), false, true);
    {
        VertexData::Mapper mapper(data);
        for (size_t i = 0; i < data.num_vertices(); ++i) {
            auto p = mapper.Get<VertexXYUV>(i);
            p->set_xy(ps[i]);
            std::tie(p->u, p->v) = uv[i];
        }
    }
    canvas.SendVertexData(data, VertexType::VERTEX, 0);
    canvas.SendVertexData(data, VertexType::TEXTURE, sizeof(float) * 2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void DrawCircle(const geometry::Circle* circle, const Vector2D& position, ugdk::graphic::Canvas& canvas)
{
    TextureUnit unit(graphic::manager()->ReserveTextureUnit(graphic::manager()->white_texture()));
    canvas.SendUniform("drawable_texture", unit);
    
    math::Vector2D origin = core::FromWorldCoordinates(position);

    VertexData data(10, sizeof(VertexXYUV), false, true);
    {
        VertexData::Mapper mapper(data);
        {
            auto p = mapper.Get<VertexXYUV>(0);
            p->set_xy(origin);
            p->u = p->v = 0.0f;
        }
        for (size_t i = 1; i < data.num_vertices(); ++i) {
            auto p = mapper.Get<VertexXYUV>(i);
            p->set_xy(core::FromWorldCoordinates(position + Vector2D(circle->radius(), 0.0).Rotate((i - 1) / 4.0 * PI)));
            p->u = p->v = 1.0f;
        }
    }
    canvas.SendVertexData(data, VertexType::VERTEX, 0);
    canvas.SendVertexData(data, VertexType::TEXTURE, sizeof(float)* 2);
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
