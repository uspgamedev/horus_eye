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
#include <ugdk/graphic/texture.h>
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
    vertex_shader.AddCodeBlock("out vec2 P; " "\n");
    vertex_shader.AddLineInMain("	vec4 position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n");
    vertex_shader.AddLineInMain("	gl_Position = position;" "\n");
    vertex_shader.AddLineInMain("	P = vertexPosition.xy;" "\n");
    vertex_shader.GenerateSource();

    // FRAGMENT
    fragment_shader.AddCodeBlock("in vec2 P;" "\n");
    fragment_shader.AddCodeBlock("uniform vec2 O;" "\n");
    fragment_shader.AddCodeBlock("uniform vec2 A;" "\n");
    fragment_shader.AddCodeBlock("uniform vec2 B;" "\n");
    fragment_shader.AddLineInMain(" vec2 OP = P - O;" "\n");
    fragment_shader.AddLineInMain(" vec2 AB = B - A;" "\n");
    fragment_shader.AddLineInMain(" float angle = (O.y + OP.y * ((A.x - O.x) / OP.x) - A.y) / (AB.y - ((OP.y * AB.x) / OP.x));" "\n");
    fragment_shader.AddLineInMain(" float alpha = 1.0 - abs(1.0 - 2 * angle);" "\n");
    fragment_shader.AddLineInMain(" alpha = clamp(0.25 + 2 * alpha, 0.0, 1.0);" "\n");
    fragment_shader.AddLineInMain(" gl_FragColor = vec4(alpha, alpha, alpha, 1.0);" "\n");
    fragment_shader.GenerateSource();

    horus_shadowcasting_shader_ = new opengl::ShaderProgram;

    horus_shadowcasting_shader_->AttachShader(vertex_shader);
    horus_shadowcasting_shader_->AttachShader(fragment_shader);

    bool status = horus_shadowcasting_shader_->SetupProgram();
    assert(status);
}

ugdk::graphic::Texture* shadow_texture_ = nullptr;

bool light_system_activated = true;
bool shadowcasting_actiavated = true;

void DrawQuadrilateral(const math::Vector2D& p1, const math::Vector2D& p2, 
                       const math::Vector2D& p3, const math::Vector2D& p4,
                       const math::Vector2D& O,
                       const Geometry& geometry, const VisualEffect& effect) {
    //opengl::ShaderProgram::Use shader_use(horus_shadowcasting_shader_);
    opengl::ShaderProgram::Use shader_use(horus_shadowcasting_shader_);
    shader_use.SendGeometry(geometry);
    //shader_use.SendEffect(effect);
    //shader_use.SendTexture(0, graphic::manager()->white_texture());

    math::Vector2D p1s = core::FromWorldCoordinates(p1),
                   p2s = core::FromWorldCoordinates(p2),
                   p3s = core::FromWorldCoordinates(p3),
                   p4s = core::FromWorldCoordinates(p4),
                   Os = core::FromWorldCoordinates(O);

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
        for(int i = 0; i < 4; ++i)
            vertex_data[i] = 0.0f;
        for(int i = 4; i < 8; ++i)
            vertex_data[i] = 1.0f;
    }

    shader_use.SendUniform("O", float(Os.x),  float(Os.y));
    shader_use.SendUniform("A", float(p2s.x), float(p2s.y));
    shader_use.SendUniform("B", float(p3s.x), float(p3s.y));

    shader_use.SendVertexBuffer(&vertexbuffer, opengl::VERTEX, 0);
    shader_use.SendVertexBuffer(&colorbuffer, opengl::TEXTURE, 0);
    glDrawArrays(GL_QUADS, 0, 4);
}

void CreateAndDrawQuadrilateral(const Geometry& geometry, const VisualEffect& effect, const math::Vector2D& from, const math::Vector2D& left_point, const math::Vector2D& right_point) {
    using math::Vector2D;
    Vector2D wall_dir = (right_point - left_point).Normalize();
    Vector2D left_vector = left_point - from;
    Vector2D right_vector = right_point - from;

    static const double near_distance = 0.02;
    static const double far_distance = 10.0;

    double near_coef_left   = near_distance / (left_vector - left_vector*wall_dir*wall_dir).length();
    double near_coef_right  = near_distance / (right_vector - right_vector*wall_dir*wall_dir).length();
    double far_coef_left    = far_distance / (left_vector - left_vector*wall_dir*wall_dir).length();
    double far_coef_right   = far_distance / (right_vector - right_vector*wall_dir*wall_dir).length();

    DrawQuadrilateral(
        left_point  + left_vector   * far_coef_left, // far left
        left_point  + left_vector   * near_coef_left, // near left
        right_point + right_vector  * near_coef_right, // near right
        right_point + right_vector  * far_coef_right, // far right
        from,
        geometry, effect);
}

bool is_inside(const geometry::GeometricShape* shape, const Vector2D& position, const Vector2D& point) {
    geometry::Circle c(0.0);
    return shape->Intersects(position, &c, point);
}

void DrawShadows(scene::World* world, sprite::WorldObject* hero, const Geometry& geometry, const VisualEffect& effect) {
    auto& opaque_class = world->visibility_manager()->Find("Opaque");
    geometry::Rect screen_rect(20.0, 20.0); // TODO: Get size from screen size
    collision::CollisionObjectList walls;
    opaque_class.FindCollidingObjects(hero->world_position(), screen_rect, walls);

    Geometry offset_geometry = geometry * world->camera();
    VisualEffect black_effect = effect * VisualEffect(Color(0.0, 0.0, 0.0, 0.5));

    for(const collision::CollisionObject * obj : walls) {
        std::list<Vector2D> vertices;
        if(const geometry::Rect* wall_rect = dynamic_cast<const geometry::Rect*>(obj->shape())) {
            // OH YEAHHHHHH
            math::Vector2D top_left = obj->absolute_position() 
                + math::Vector2D(wall_rect->width(), wall_rect->height()) * 0.5;
            math::Vector2D bottom_right = obj->absolute_position() 
                - math::Vector2D(wall_rect->width(), wall_rect->height()) * 0.5;

            vertices.emplace_back(    top_left.x,     top_left.y);
            vertices.emplace_back(bottom_right.x,     top_left.y);
            vertices.emplace_back(bottom_right.x, bottom_right.y);
            vertices.emplace_back(    top_left.x, bottom_right.y);

        } else {
            puts("PAREDE COM SHAPE QUE N�O � RECT, QUE MERDA ROLOOOOOOOOU?!?!?!"); // TODO: handle better
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

void LightRendering(const Geometry& geometry, const VisualEffect& effect) {
    if(scene::World* world = utils::LevelManager::reference()->current_level()) {
        if(shadowcasting_actiavated)
            if(sprite::WorldObject* hero = world->hero())
                DrawShadows(world, hero, geometry, effect);
        graphic::manager()->SaveBackbufferToTexture(shadow_texture_);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if(light_system_activated)
            world->RenderLight(geometry, effect);
        else {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        }
        
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
        if(shadowcasting_actiavated)
            DrawTexture(shadow_texture_, geometry, effect);
    }
}

} // namespace

ugdk::graphic::opengl::ShaderProgram* get_horus_light_shader() { return horus_light_shader_; }

void AddHorusShader() {
    AddHorusLightShader();
    AddShadowcastingShader();
}

void ToggleShadowcasting() {
   shadowcasting_actiavated = !shadowcasting_actiavated;
}

void ToggleLightsystem() {
    light_system_activated = !light_system_activated;
}

ugdk::action::Scene* CreateHorusLightrenderingScene() {
    auto screensize = graphic::manager()->video_size();
    shadow_texture_ = ugdk::graphic::Texture::CreateRawTexture(screensize.x, screensize.y);
    glBindTexture(GL_TEXTURE_2D, shadow_texture_->gltexture());
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, shadow_texture_->width(),
        shadow_texture_->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return ugdk::graphic::CreateLightrenderingScene(LightRendering);
}

void DrawTexture(ugdk::graphic::Texture* texture, const Geometry& geometry, const VisualEffect& effect) {
    opengl::ShaderProgram::Use shader_use(graphic::manager()->shaders().GetSpecificShader(0));
    shader_use.SendGeometry(geometry);
    shader_use.SendEffect(effect);
    shader_use.SendTexture(0, texture);
    
    opengl::VertexArray vertexbuffer(sizeof(GLfloat) * 2 * 4, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Mapper mapper(vertexbuffer);
        GLfloat *vertex_data = static_cast<GLfloat*>(mapper.get());
        vertex_data[0 * 2 + 0] = 0.0f; // near left
        vertex_data[0 * 2 + 1] = static_cast<GLfloat>(texture->height());
        
        vertex_data[1 * 2 + 0] = static_cast<GLfloat>(texture->width()); // near right
        vertex_data[1 * 2 + 1] = static_cast<GLfloat>(texture->height());
        
        vertex_data[2 * 2 + 0] = static_cast<GLfloat>(texture->width()); // far right
        vertex_data[2 * 2 + 1] = 0.0f;

        vertex_data[3 * 2 + 0] = 0.0f; // far left
        vertex_data[3 * 2 + 1] = 0.0f;
    }
    shader_use.SendVertexBuffer(&vertexbuffer, opengl::VERTEX, 0);
    shader_use.SendVertexBuffer(opengl::VertexBuffer::CreateDefault(), opengl::TEXTURE, 0);
    glDrawArrays(GL_QUADS, 0, 4);
}
