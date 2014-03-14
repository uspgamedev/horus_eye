#include "game/components/graphic.h"

#include <glm/glm.hpp>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/sprite.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/math/vector2D.h>

#include "game/core/coordinates.h"
#include "game/components/animator.h"
#include "game/sprites/worldobject.h"
#include "game/initializer.h"

#define LIGHT_COEFFICIENT 0.75

namespace component {

using ugdk::math::Vector2D;
using ugdk::graphic::Node;
using ugdk::graphic::Drawable;

Graphic::Graphic(const std::shared_ptr<ugdk::graphic::Primitive>& primitive, Animator* animator)
: primitive_(primitive)
, animator_(animator)
, layer_(scene::FOREGROUND_LAYER)
{}

Graphic::~Graphic() {
    delete animator_;
}
    
void Graphic::set_primitive(const std::shared_ptr<ugdk::graphic::Primitive>& primitive) {
    primitive_ = primitive;
}


void Graphic::SetPosition(const ugdk::math::Vector2D& position) {
    world_position_ = position;
    final_position_ = core::FromWorldCoordinates(world_position_) + render_offset_;
}

void Graphic::set_render_offset(const ugdk::math::Vector2D& render_offset) {
    render_offset_ = render_offset;
    final_position_ = core::FromWorldCoordinates(world_position_) + render_offset_;
}

double Graphic::alpha() const {
    return visual_effect_.color().a;
}

void Graphic::ChangeAlpha(double alpha) {
    ugdk::Color color = visual_effect_.color();
    color.a = alpha;
    visual_effect_.set_color(color);
}

bool Graphic::visible() const {
    return visual_effect_.visible();
}

void Graphic::set_visible(bool visible) {
    visual_effect_.set_visible(visible);
}

void Graphic::Update(double dt) {
    if(animator_)
        animator_->sprite()->animation_player().Update(dt);
}

void Graphic::Render(ugdk::graphic::Canvas& canvas) const {
    if (primitive_) {
        canvas.PushAndCompose(final_position_);

        const ugdk::graphic::Geometry& geo = canvas.current_geometry();
        glm::vec4 render_ogl = geo.AsMat4() * glm::vec4(render_offset_.x, render_offset_.y, 0.0, 0.0);
        Vector2D lightpos = (geo.offset() - Vector2D(render_ogl.x, render_ogl.y))* 0.5 + Vector2D(0.5, 0.5);

        {
            ugdk::graphic::opengl::ShaderUse shader(get_horus_light_shader());
            shader.SendUniform("lightUV", lightpos.x, lightpos.y);
        }

        primitive_->Draw(canvas);
        canvas.PopGeometry();
    }
}
    
void Graphic::OnAdd(sprite::WorldObject* wobj) {
    SetPosition(wobj->world_position());
}
    
Graphic* Graphic::Create(const std::shared_ptr<ugdk::graphic::Primitive>& primitive) {
    return new Graphic(primitive, nullptr);
}

Graphic* Graphic::Create(Animator* animator) {
    if(!animator) return new Graphic(nullptr, nullptr);
    return new Graphic(animator->sprite()->primitive(), animator);
}
    
Graphic* Graphic::Create(const std::string& spritesheet_name, const std::string& animation_set) {
    return Create(new Animator(spritesheet_name, animation_set));
}

}  // namespace component
