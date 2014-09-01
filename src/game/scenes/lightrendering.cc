#include "lightrendering.h"

#include <ugdk/system/engine.h>
#include <ugdk/internal/opengl.h>
#include <ugdk/debug/profiler.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/rendertexture.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/drawable/functions.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <pyramidworks/collision/collisionclass.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>

#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"


// debug
#include <ugdk/input/module.h>

namespace scene {

using namespace ugdk;
using namespace ugdk::graphic;
using namespace pyramidworks;
using ugdk::math::Vector2D;

namespace {
    uint16 quad_to_triangles_indices[] = { 0, 1, 2, 0, 2, 3 };
    double LIGHT_PRECISION = 32.0;

    struct VertexXYUV {
        float x, y, u, v;
        void set_xy(const math::Vector2D& v) {
            x = static_cast<float>(v.x);
            y = static_cast<float>(v.y);
        }
        void set_xyuv(float _x, float _y, float _u, float _v) {
            x = _x;
            y = _y;
            u = _u;
            v = _v;
        }
    };

    ugdk::graphic::opengl::ShaderProgram* horus_shadowcasting_shader_ = nullptr;
    void AddShadowcastingShader() {
        opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        // VERTEX
        vertex_shader.AddCodeBlock("out highp vec2 P; " "\n");
        vertex_shader.AddLineInMain("   highp vec4 position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n");
        vertex_shader.AddLineInMain("   gl_Position = position;" "\n");
        vertex_shader.AddLineInMain("   P = vertexPosition.xy;" "\n");
        vertex_shader.GenerateSource();

        // FRAGMENT
        fragment_shader.AddCodeBlock("in highp vec2 P;" "\n");
        fragment_shader.AddCodeBlock("uniform highp vec2 O;" "\n");
        fragment_shader.AddCodeBlock("uniform highp vec2 A;" "\n");
        fragment_shader.AddCodeBlock("uniform highp vec2 B;" "\n");
        fragment_shader.AddLineInMain(" highp vec2 OP = P - O;" "\n");
        fragment_shader.AddLineInMain(" highp vec2 AB = B - A;" "\n");
        fragment_shader.AddLineInMain(" highp float angle = (O.y + OP.y * ((A.x - O.x) / OP.x) - A.y) / (AB.y - ((OP.y * AB.x) / OP.x));" "\n");
        fragment_shader.AddLineInMain(" highp float alpha = 1.0 - abs(1.0 - 2.0 * angle);" "\n");
        fragment_shader.AddLineInMain(" alpha = clamp(0.25 + 2.0 * alpha, 0.0, 1.0);" "\n");
        fragment_shader.AddLineInMain(" alpha = 1.0;" "\n");
        fragment_shader.AddLineInMain(" gl_FragColor = vec4(alpha, alpha, alpha, 1.0);" "\n");
        fragment_shader.GenerateSource();

        horus_shadowcasting_shader_ = new opengl::ShaderProgram;

        horus_shadowcasting_shader_->AttachShader(vertex_shader);
        horus_shadowcasting_shader_->AttachShader(fragment_shader);

        bool status = horus_shadowcasting_shader_->SetupProgram();
        assert(status);
    }


    void DrawQuadrilateral(const math::Vector2D& p1, const math::Vector2D& p2,
                           const math::Vector2D& p3, const math::Vector2D& p4,
                           const math::Vector2D& O,
                           ugdk::graphic::Canvas& canvas)
    {
        VertexData data(4, sizeof(VertexXYUV), false, true);
        {
            VertexData::Mapper mapper(data);
            {
                auto p = mapper.Get<VertexXYUV>(0);
                p->set_xy(p1);
                p->u = p->v = 0.0f;
            }
            {
                auto p = mapper.Get<VertexXYUV>(1);
                p->set_xy(p2);
                p->u = p->v = 0.0f;
            }
            {
                auto p = mapper.Get<VertexXYUV>(2);
                p->set_xy(p3);
                p->u = p->v = 1.0f;
            }
            {
                auto p = mapper.Get<VertexXYUV>(3);
                p->set_xy(p4);
                p->u = p->v = 1.0f;
            }
        }

        canvas.SendUniform("O", float(O.x), float(O.y));
        canvas.SendUniform("A", float(p2.x), float(p2.y));
        canvas.SendUniform("B", float(p3.x), float(p3.y));

        canvas.SendVertexData(data, VertexType::VERTEX, 0);
        canvas.SendVertexData(data, VertexType::TEXTURE, sizeof(float)* 2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void CreateAndDrawQuadrilateral(ugdk::graphic::Canvas& canvas, const math::Vector2D& from, const math::Vector2D& left_point, const math::Vector2D& right_point) {
        using math::Vector2D;
        Vector2D wall_dir = (right_point - left_point).Normalize();
        Vector2D left_vector = left_point - from;
        Vector2D right_vector = right_point - from;

        static const double near_distance = 0.02;
        static const double far_distance = 10.0;

        double near_coef_left = near_distance / (left_vector - left_vector*wall_dir*wall_dir).length();
        double near_coef_right = near_distance / (right_vector - right_vector*wall_dir*wall_dir).length();
        double far_coef_left = far_distance / (left_vector - left_vector*wall_dir*wall_dir).length();
        double far_coef_right = far_distance / (right_vector - right_vector*wall_dir*wall_dir).length();

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

    void DrawShadows(scene::World* world, sprite::WorldObject* hero, ugdk::graphic::Canvas& canvas) {
        ugdk::debug::ProfileSection section("DrawShadows");

        auto& opaque_class = world->visibility_manager()->Find("Opaque");
        geometry::Rect screen_rect(20.0, 20.0); // TODO: Get size from screen size
        collision::CollisionObjectList walls;
        opaque_class.FindCollidingObjects(hero->world_position(), screen_rect, walls);

        canvas.PushAndCompose(VisualEffect(Color(0.0, 0.0, 0.0, 0.5)));

        for (const collision::CollisionObject * obj : walls) {
            std::list<Vector2D> vertices;
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

            std::list<Vector2D> extremes;
            for (const Vector2D& vertex : vertices) {
                Vector2D dir = (vertex - hero->world_position()).Normalize();
                if (!is_inside(obj->shape(), obj->absolute_position(), vertex + dir * 0.01)
                    && !is_inside(obj->shape(), obj->absolute_position(), vertex - dir * 0.01))
                    extremes.push_back(vertex);
            }

            // Choose the two points that have the widest angle.
            if (!extremes.empty())
                CreateAndDrawQuadrilateral(canvas, hero->world_position(), extremes.front(), extremes.back());
        }

        canvas.PopVisualEffect();
    }

    void DrawBuffer(Canvas& canvas, ugdk::graphic::RenderTexture& buffer) {
        VertexData data(4, sizeof(VertexXYUV), false, true);
        {
            VertexData::Mapper mapper(data);
            mapper.Get<VertexXYUV>(0)->set_xyuv(0.0f, 0.0f, 0.0f, 0.0f);
            mapper.Get<VertexXYUV>(1)->set_xyuv(buffer.size().x, 0.0f, 1.0f, 0.0f);
            mapper.Get<VertexXYUV>(2)->set_xyuv(0.0f, buffer.size().y, 0.0f, 1.0f);
            mapper.Get<VertexXYUV>(3)->set_xyuv(buffer.size().x, buffer.size().y, 1.0f, 1.0f);
        }
        TextureUnit unit = graphic::manager()->ReserveTextureUnit(buffer.texture());
        canvas.SendUniform("drawable_texture", unit);
        canvas.SendVertexData(data, VertexType::VERTEX, 0);
        canvas.SendVertexData(data, VertexType::TEXTURE, sizeof(float)* 2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

}


LightRendering::LightRendering(World* world)
: shadow_buffer_(world->size() * LIGHT_PRECISION)
, light_buffer_(world->size() * LIGHT_PRECISION)
, shadowcasting_actiavated_(true)
, lightsystem_activated_(true)
, world_(world)
{
    this->set_identifier("Light Rendering");
    this->set_focus_callback(std::mem_fn(&Scene::Finish));

    printf("Creating LightRendering. Size %d, %d\n", world->size().x, world->size().y);

    Geometry project_matrix(math::Vector2D(-1.0, -1.0), math::Vector2D(2.0/world->size().x, 2.0/world->size().y));
    shadow_buffer_.set_projection_matrix(project_matrix);
    light_buffer_.set_projection_matrix(project_matrix);

    if (!horus_shadowcasting_shader_)
        AddShadowcastingShader();

    this->set_render_function([this](Canvas& canvas) {
        // Render the shadows.

        //if (shadowcasting_actiavated_) {
            ShadowCasting();
            if (input::manager()->keyboard().IsDown(input::Keycode::n))
                DrawBuffer(canvas, shadow_buffer_);
        //}

        // Lights are simply added together.
        if (lightsystem_activated_) {
            Canvas light_canvas(&light_buffer_);
            light_canvas.Clear(Color(.0, .0, .0, 1.0));

            glBlendFunc(GL_ONE, GL_ONE);
            world_->RenderLight(light_canvas);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            if (shadowcasting_actiavated_)
                ApplyShadowCasting(light_canvas);

        } else {
            Canvas light_canvas(&light_buffer_);
            light_canvas.Clear(Color(1.0, 1.0, 1.0, 1.0));
        }

        if (input::manager()->keyboard().IsDown(input::Keycode::b)) {
            canvas.PushAndCompose(Color(1, 0, 0, 0.5));
            DrawBuffer(canvas, light_buffer_);
            canvas.PopVisualEffect();
        }
    });
}

LightRendering::~LightRendering() {}

void LightRendering::ToggleShadowcasting() {
    shadowcasting_actiavated_ = !shadowcasting_actiavated_;
}

void LightRendering::ToggleLightsystem() {
    lightsystem_activated_ = !lightsystem_activated_;
}

const ugdk::internal::GLTexture* LightRendering::light_texture() const {
    return light_buffer_.texture();
}

void LightRendering::ShadowCasting() {
    Canvas canvas(&shadow_buffer_);
    canvas.Clear(Color(0.0, 0.0, 0.0, 0.0));
    canvas.ChangeShaderProgram(horus_shadowcasting_shader_);

    glBlendFunc(GL_ONE, GL_ONE);
    if (sprite::WObjPtr hero = world_->hero().lock())
        DrawShadows(world_, hero.get(), canvas);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void LightRendering::ApplyShadowCasting(Canvas& canvas) {
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    ugdk::debug::ProfileSection section("DrawShadowTexture");
    canvas.ChangeShaderProgram(graphic::manager()->shaders().GetSpecificShader(0));
    DrawBuffer(canvas, shadow_buffer_);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
    
} // namespace scene
