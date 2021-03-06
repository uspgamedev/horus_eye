#include "lightrendering.h"

#include <ugdk/action/events.h>
#include <ugdk/system/engine.h>
#include <ugdk/debug/profiler.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/rendertexture.h>
#include <ugdk/structure/visualeffect.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/shader.h>
#include <ugdk/graphic/shaderprogram.h>
#include <pyramidworks/collision/collisionclass.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>

#include "game/core/world.h"
#include "game/map/room.h"
#include "game/sprites/worldobject.h"
#include "game/components/lightemitter.h"

namespace frontend {
namespace gameview {

using namespace ugdk;
using namespace ugdk::graphic;
using namespace pyramidworks;
using ugdk::math::Vector2D;

namespace {
    double LIGHT_PRECISION = 32.0;
    math::Vector2D DIMENSIONS(12, 12);

    class Line {
      public:
        Line(const math::Vector2D& p1, const math::Vector2D& p2)
            : a_(p1.y - p2.y)
            , b_(p2.x - p1.x)
            , c_(p1.x * p2.y - p2.x * p1.y)
        {}

        bool IsAbove(const math::Vector2D& v) const {
            return a_ * v.x + b_ * v.y + c_ >= 0.0;
        }

      private:
        double a_, b_, c_;
    };

    struct VertexXY {
        glm::vec2 position;
        void set_xy(const math::Vector2D& v) {
            position.x = static_cast<float>(v.x);
            position.y = static_cast<float>(v.y);
        }
    };

    struct VertexXYUV {
        glm::vec2 position;
        glm::vec2 texture;
        void set_xy(const math::Vector2D& v) {
            position.x = static_cast<float>(v.x);
            position.y = static_cast<float>(v.y);
        }
        void set_xyuv(float _x, float _y, float _u, float _v) {
            position.x = _x;
            position.y = _y;
            texture.x = _u;
            texture.y = _v;
        }
    };

    ShaderProgram* horus_shadowcasting_shader_ = nullptr;
    void AddShadowcastingShader() {
        Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        // VERTEX
        vertex_shader.AddLineInMain("   highp vec4 position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n");
        vertex_shader.AddLineInMain("   gl_Position = position;" "\n");
        vertex_shader.GenerateSource();

        // FRAGMENT
        fragment_shader.AddLineInMain(" gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);" "\n");
        fragment_shader.GenerateSource();

        horus_shadowcasting_shader_ = new ShaderProgram;

        horus_shadowcasting_shader_->AttachShader(vertex_shader);
        horus_shadowcasting_shader_->AttachShader(fragment_shader);

        bool status = horus_shadowcasting_shader_->SetupProgram();
        assert(status);
    }


    void DrawQuadrilateral(const math::Vector2D& far_left, const math::Vector2D& near_left,
                           const math::Vector2D& near_right, const math::Vector2D& far_right,
                           const math::Vector2D& O,
                           ugdk::graphic::Canvas& canvas)
    {
        VertexData data(4, sizeof(VertexXY), false, true);
        {
            VertexData::Mapper mapper(data);
            mapper.Get<VertexXY>(0)->set_xy(far_left);
            mapper.Get<VertexXY>(1)->set_xy(far_right);
            mapper.Get<VertexXY>(2)->set_xy(near_left);
            mapper.Get<VertexXY>(3)->set_xy(near_right);
        }

        canvas.SendVertexData(data, VertexType::VERTEX, 0);
        manager()->DisableVertexType(VertexType::TEXTURE);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void CreateAndDrawQuadrilateral(ugdk::graphic::Canvas& canvas, const math::Vector2D& from, const math::Vector2D& left_point, const math::Vector2D& right_point) {
        using math::Vector2D;
        Vector2D wall_dir = (right_point - left_point).Normalize();
        Vector2D left_vector = left_point - from;
        Vector2D right_vector = right_point - from;

        static const double near_distance = 0.0;
        static const double far_distance = 15.0;

        double near_coef_left   =  near_distance / (left_vector - left_vector*wall_dir*wall_dir).length();
        double near_coef_right  =  near_distance / (right_vector - right_vector*wall_dir*wall_dir).length();
        double far_coef_left    =   far_distance / (left_vector - left_vector*wall_dir*wall_dir).length();
        double far_coef_right   =   far_distance / (right_vector - right_vector*wall_dir*wall_dir).length();

        DrawQuadrilateral(
            left_point + left_vector   * far_coef_left, // far left
            left_point + left_vector   * near_coef_left, // near left
            right_point + right_vector  * near_coef_right, // near right
            right_point + right_vector  * far_coef_right, // far right
            from,
            canvas);
    }

    bool is_inside(const geometry::GeometricShape* shape, const Vector2D& position, const Vector2D& point) {
        geometry::Circle c(0.0);
        return shape->Intersects(position, &c, point);
    }

    void DrawShadows(core::World* world, sprite::WObjRawPtr hero, ugdk::graphic::Canvas& canvas) {
        ugdk::debug::ProfileSection section("DrawShadows");

        auto& opaque_class = world->visibility_manager()->Find("Opaque");
        geometry::Rect screen_rect(20.0, 20.0); // TODO: Get size from screen size
        collision::CollisionObjectList walls;
        opaque_class.FindCollidingObjects(hero->world_position(), screen_rect, walls);

        canvas.PushAndCompose(structure::VisualEffect(structure::Color(0.0, 0.0, 0.0, 0.5)));

        for (const collision::CollisionObject * obj : walls) {
            std::vector<Vector2D> vertices;
            vertices.reserve(4);
            if (const geometry::Rect* wall_rect = dynamic_cast<const geometry::Rect*>(obj->shape())) {
                Vector2D wall_size(wall_rect->width(), wall_rect->height());
                Vector2D top_left = obj->absolute_position() - wall_size * 0.5;
                Vector2D bottom_right = obj->absolute_position() + wall_size * 0.5;

                vertices.emplace_back(top_left.x, top_left.y);
                vertices.emplace_back(bottom_right.x, top_left.y);
                vertices.emplace_back(bottom_right.x, bottom_right.y);
                vertices.emplace_back(top_left.x, bottom_right.y);

            } else {
                puts("PAREDE COM SHAPE QUE NÃO É RECT, QUE MERDA ROLOOOOOOOOU?!?!?!"); // TODO: handle better
            }

            std::list<int> extremes;
            for (size_t i = 0; i < vertices.size(); ++i) {
                const auto& vertex = vertices[i];
                Vector2D dir = (vertex - hero->world_position()).Normalize();
                if (!is_inside(obj->shape(), obj->absolute_position(), vertex + dir * 0.01)
                    && !is_inside(obj->shape(), obj->absolute_position(), vertex - dir * 0.01))
                    extremes.push_back(static_cast<int>(i));
            }

            // Choose the two points that have the widest angle.
            if (!extremes.empty()) {
                Line l(vertices[extremes.front()], vertices[extremes.back()]);
                bool eyeRelative = l.IsAbove(hero->world_position());

                int startV = extremes.front();
                int endV = extremes.back();
                int numV = static_cast<int>(vertices.size());

                bool shouldInvert = false;

                // Checking if we should invert startV and endV.
                if (startV + 1 != endV) { // if there's a vertex in the middle to check, yay
                    shouldInvert = l.IsAbove(vertices[startV + 1]) == eyeRelative;
                } else {
                    shouldInvert = l.IsAbove(vertices[(endV + 1) % numV]) != eyeRelative;
                }
                if (shouldInvert) {
                    startV = endV;
                    endV = extremes.front() + numV;
                }
                for (int i = startV; i < endV; ++i) {
                    CreateAndDrawQuadrilateral(canvas, hero->world_position(), vertices[i % numV], vertices[(i + 1) % numV]);
                }
            }
        }

        canvas.PopVisualEffect();
    }

    void RenderRoomLight(const map::Room* room, ugdk::graphic::Canvas& canvas) {
        ugdk::debug::ProfileSection section("Room '" + room->name() + "'");

        for (const auto& obj : *room)
            if (const auto& light_comp = obj->light())
                light_comp->Render(canvas);
    }

    void RenderLight(core::World* world, ugdk::graphic::Canvas& canvas) {
        ugdk::debug::ProfileSection section("World::RenderLight");
        for (const map::Room* room : world->active_rooms())
            RenderRoomLight(room, canvas);
    }
}


LightRendering::LightRendering(core::World* world)
: shadow_buffer_(math::Integer2D(2 * DIMENSIONS * LIGHT_PRECISION))
, light_buffer_(math::Integer2D(2 * DIMENSIONS * LIGHT_PRECISION))
, shadowcasting_actiavated_(true)
, lightsystem_activated_(true)
, world_(world)
, light_precision_(DIMENSIONS)
{
    //Geometry project_matrix(math::Vector2D(-1.0, -1.0), math::Vector2D(2.0 / (dimensions_.x * 2), 2.0 / (dimensions_.y * 2)));
    math::Geometry project_matrix(math::Vector2D(.0, .0), math::Vector2D(1.0 / light_precision_.x, 1.0 / light_precision_.y));
    shadow_buffer_.set_projection_matrix(project_matrix);
    light_buffer_.set_projection_matrix(project_matrix);

    if (!horus_shadowcasting_shader_)
        AddShadowcastingShader();
}

LightRendering::~LightRendering() {}

void LightRendering::UpdateBuffers() {
    ugdk::debug::ProfileSection section("LightRendering::UpdateBuffers");

    // Render the shadows.
    if (shadowcasting_actiavated_) {
        ShadowCasting();
    }

    // Lights are simply added together.
    if (lightsystem_activated_) {
        Canvas light_canvas(&light_buffer_);
        light_canvas.Clear(structure::Color(.0, .0, .0, 1.0));
        
        glBlendFunc(GL_ONE, GL_ONE);
        light_canvas.PushAndCompose(math::Geometry(-focused_position_));
        RenderLight(world_, light_canvas);
        light_canvas.PopGeometry();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (shadowcasting_actiavated_)
            ApplyShadowCasting(light_canvas);

    } else {
        Canvas light_canvas(&light_buffer_);
        light_canvas.Clear(structure::Color(1.0, 1.0, 1.0, 1.0));
    }
}

void LightRendering::ToggleShadowcasting() {
    shadowcasting_actiavated_ = !shadowcasting_actiavated_;
}

void LightRendering::ToggleLightsystem() {
    lightsystem_activated_ = !lightsystem_activated_;
}

const GLTexture* LightRendering::light_texture() const {
    return light_buffer_.texture();
}

const GLTexture* LightRendering::shadow_texture() const {
    return shadow_buffer_.texture();
}

ugdk::math::Vector2D LightRendering::CalculateUV(const ugdk::math::Vector2D& pos) const {
    // The division maps visible lights to a [-1; 1] range, which we then change to a [0; 1] range.
    return (pos - focused_position_).Divided(light_precision_) * 0.5 + math::Vector2D(0.5, 0.5);
}

void LightRendering::ShadowCasting() {
    Canvas canvas(&shadow_buffer_);
    canvas.Clear(structure::Color(0.0, 0.0, 0.0, 0.0));
    canvas.ChangeShaderProgram(horus_shadowcasting_shader_);
    canvas.PushAndCompose(math::Geometry(-focused_position_));

    glBlendFunc(GL_ONE, GL_ONE);
    if (sprite::WObjPtr hero = world_->hero().lock())
        DrawShadows(world_, hero.get(), canvas);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void LightRendering::ApplyShadowCasting(Canvas& canvas) {
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    ugdk::debug::ProfileSection section("DrawShadowTexture");
    canvas.ChangeShaderProgram(graphic::manager()->shaders().GetSpecificShader(0));

    VertexData data(4, sizeof(VertexXYUV), false, true);
    {
        VertexData::Mapper mapper(data);
        mapper.Get<VertexXYUV>(0)->set_xyuv(-light_precision_.x,-light_precision_.y, 0.0f, 0.0f);
        mapper.Get<VertexXYUV>(1)->set_xyuv( light_precision_.x,-light_precision_.y, 1.0f, 0.0f);
        mapper.Get<VertexXYUV>(2)->set_xyuv(-light_precision_.x, light_precision_.y, 0.0f, 1.0f);
        mapper.Get<VertexXYUV>(3)->set_xyuv( light_precision_.x, light_precision_.y, 1.0f, 1.0f);
    }
    TextureUnit unit = graphic::manager()->ReserveTextureUnit(shadow_buffer_.texture());
    canvas.SendUniform("drawable_texture", unit);
    canvas.SendVertexData(data, VertexType::VERTEX, offsetof(VertexXYUV, position));
    canvas.SendVertexData(data, VertexType::TEXTURE, offsetof(VertexXYUV, texture));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
    
} // namespace frontend
} // namespace gameview
