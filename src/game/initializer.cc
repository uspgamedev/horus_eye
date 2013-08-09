#include <sys/stat.h>
#include <cerrno>
#include "initializer.h"

#include <ugdk/system/engine.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionclass.h>
#include <pyramidworks/geometry/rect.h>
#include <pyramidworks/geometry/circle.h>
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/core/coordinates.h"
    
using namespace ugdk;
using namespace ugdk::graphic;
using namespace pyramidworks;

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

ugdk::graphic::opengl::ShaderProgram* horus_light_shader_ = NULL;
void AddHorusLightShader() {
    opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

    // VERTEX
    vertex_shader.AddCodeBlock("out vec2 UV;" "\n");
    vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n");
    vertex_shader.AddLineInMain("	UV = vertexUV;" "\n");
    vertex_shader.GenerateSource();

    // FRAGMENT
    fragment_shader.AddCodeBlock("in vec2 UV;" "\n"
                                 "uniform sampler2D drawable_texture;" "\n"
                                 "uniform vec4 effect_color;" "\n");

    fragment_shader.AddCodeBlock("uniform vec2 lightUV;" "\n"
                                 "uniform sampler2D light_texture;" "\n");

    fragment_shader.AddLineInMain("	vec4 color = texture2D( drawable_texture, UV ) * effect_color;" "\n");
    fragment_shader.AddLineInMain("	color *= vec4(texture2D(light_texture, lightUV).rgb, 1.0);" "\n");
    fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
    fragment_shader.GenerateSource();

    horus_light_shader_ = new opengl::ShaderProgram;

    horus_light_shader_->AttachShader(vertex_shader);
    horus_light_shader_->AttachShader(fragment_shader);

    bool status = horus_light_shader_->SetupProgram();
    assert(status);

    ugdk::graphic::manager()->shaders().ReplaceShader((1 << 0) + (0 << 1), horus_light_shader_);
}

ugdk::graphic::opengl::ShaderProgram* horus_shadowcasting_shader_ = NULL;
void AddShadowcastingShader() {
    opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

    // VERTEX
    vertex_shader.AddCodeBlock("out float color;" "\n");
    vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n");
    vertex_shader.AddLineInMain("	color = vertexUV.x;" "\n");
    vertex_shader.GenerateSource();

    // FRAGMENT
    fragment_shader.AddCodeBlock("in float color;" "\n");
    fragment_shader.AddLineInMain(" gl_FragColor = vec4(0.0, 0.0, 0.0, color);" "\n");
    fragment_shader.GenerateSource();

    horus_shadowcasting_shader_ = new opengl::ShaderProgram;

    horus_shadowcasting_shader_->AttachShader(vertex_shader);
    horus_shadowcasting_shader_->AttachShader(fragment_shader);

    bool status = horus_shadowcasting_shader_->SetupProgram();
    assert(status);
    glBindAttribLocation(horus_shadowcasting_shader_->id(), 2, "vertexAlpha");
}

}

ugdk::graphic::opengl::ShaderProgram* get_horus_light_shader() { return horus_light_shader_; }

void AddHorusShader() {
    AddHorusLightShader();
    AddShadowcastingShader();
}

void DrawQuadrilateral(const math::Vector2D& p1, const math::Vector2D& p2, 
                       const math::Vector2D& p3, const math::Vector2D& p4, 
                       const Geometry& geometry, const VisualEffect& effect) {
    //opengl::ShaderProgram::Use shader_use(horus_shadowcasting_shader_);
    opengl::ShaderProgram::Use shader_use(horus_shadowcasting_shader_);
    shader_use.SendGeometry(geometry);
    //shader_use.SendEffect(effect);
    //shader_use.SendTexture(0, graphic::manager()->white_texture());

    math::Vector2D p1s = core::FromWorldCoordinates(p1),
                   p2s = core::FromWorldCoordinates(p2),
                   p3s = core::FromWorldCoordinates(p3),
                   p4s = core::FromWorldCoordinates(p4);

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
    opengl::VertexArray colorbuffer(sizeof(GLfloat) * 2 * 4, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Mapper mapper(colorbuffer);
        GLfloat *vertex_data = static_cast<GLfloat*>(mapper.get());
        for(int i = 0; i < 8; ++i)
            vertex_data[i] = 0.0f;
        vertex_data[0] = 0.0f; // far left
        vertex_data[2] = 0.0f; // right
        vertex_data[4] = 1.0f; // bottom
        vertex_data[6] = 1.0f; // left
    }

    shader_use.SendVertexBuffer(&vertexbuffer, opengl::VERTEX, 0);
    shader_use.SendVertexBuffer(&colorbuffer, opengl::TEXTURE, 0);
    glDrawArrays(GL_QUADS, 0, 4);
}

void CreateAndDrawQuadrilateral(const Geometry& geometry, const VisualEffect& effect, const math::Vector2D& from, const math::Vector2D& left_point, const math::Vector2D& right_point) {
    math::Vector2D left_vector = (left_point - from).Normalize();
    math::Vector2D right_vector = (right_point - from).Normalize();

    static const double near_distance = 0.07;
    static const double far_distance = 20.0;

    DrawQuadrilateral(
        left_point + left_vector * far_distance, // far left
        left_point + left_vector * near_distance, // near left
        right_point + right_vector * near_distance, // near right
        right_point + right_vector * far_distance, // far right
        geometry, effect);
}

bool is_inside(const geometry::GeometricShape* shape, const Vector2D& position, const Vector2D& point) {
    geometry::Circle c(0.0);
    return shape->Intersects(position, &c, point);
}

void LightRendering(const Geometry& geometry, const VisualEffect& effect) {
    if(scene::World* world = utils::LevelManager::reference()->current_level()) {
        world->content_node()->RenderLight(geometry, effect);

        if(sprite::WorldObject* hero = world->hero()) {
            auto opaque_class = world->visibility_manager()->Get("Opaque");
            geometry::Rect screen_rect(20.0, 20.0); // TODO: Get size from screen size
            collision::CollisionObjectList walls;
            opaque_class->FindCollidingObjects(hero->world_position(), screen_rect, walls);

            Geometry offset_geometry = geometry * world->content_node()->geometry();
            VisualEffect black_effect = effect * VisualEffect(Color(0.0, 0.0, 0.0, 0.5));

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            for(const collision::CollisionObject * obj : walls) {
                std::list<Vector2D> vertices;
                if(const geometry::Rect* wall_rect = dynamic_cast<const geometry::Rect*>(obj->shape())) {
                    // OH YEAHHHHHH
                    math::Vector2D top_left = obj->absolute_position() + math::Vector2D(wall_rect->width(), wall_rect->height()) * 0.5;
                    math::Vector2D bottom_right = obj->absolute_position() - math::Vector2D(wall_rect->width(), wall_rect->height()) * 0.5;

                    vertices.emplace_back(    top_left.x,     top_left.y);
                    vertices.emplace_back(bottom_right.x,     top_left.y);
                    vertices.emplace_back(bottom_right.x, bottom_right.y);
                    vertices.emplace_back(    top_left.x, bottom_right.y);

                } else {
                    puts("PAREDE COM SHAPE QUE NÃO É RECT, QUE MERDA ROLOOOOOOOOU?!?!?!"); // TODO: handle better
                }

                std::list<Vector2D> extremes;
                for(const Vector2D& vertex : vertices) {
                    Vector2D dir = (vertex - hero->world_position()).Normalize();
                    if(!is_inside(obj->shape(), obj->absolute_position(), vertex + dir * 0.01)
                        && !is_inside(obj->shape(), obj->absolute_position(), vertex - dir * 0.01))
                        extremes.push_back(vertex);
                }

                // Choose the two points that have the widest angle.
                CreateAndDrawQuadrilateral(offset_geometry, black_effect, hero->world_position(), extremes.front(), extremes.back());
            }
        }
    }
}

ugdk::action::Scene* CreateHorusLightrenderingScene() {
    return ugdk::graphic::CreateLightrenderingScene(LightRendering);
}