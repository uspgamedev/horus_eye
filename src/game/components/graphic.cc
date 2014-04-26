#include "game/components/graphic.h"

#include <glm/glm.hpp>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/sprite.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/resource/module.h>

#include "game/core/coordinates.h"
#include "game/components/animator.h"
#include "game/sprites/worldobject.h"
#include "game/initializer.h"

namespace component {

using ugdk::math::Vector2D;
using ugdk::graphic::Node;
using ugdk::graphic::Drawable;
using ugdk::graphic::Primitive;

Graphic::Graphic(Animator* animator)
: primitive_(nullptr, nullptr)
, animator_(animator)
, layer_(scene::FOREGROUND_LAYER)
{
    if (animator_)
        animator_->Configure(this);
}

Graphic::~Graphic() {
    delete animator_;
}
    
void Graphic::SetPosition(const ugdk::math::Vector2D& position) {
    world_position_ = position;
    final_position_ = core::FromWorldCoordinates(world_position_) + render_offset_;
    if (auto controlller = dynamic_cast<ugdk::graphic::PrimitiveControllerPosition*>(primitive_.controller().get())) {
        controlller->ChangePosition(final_position_);
    }
    
}

void Graphic::set_render_offset(const ugdk::math::Vector2D& render_offset) {
    render_offset_ = render_offset;
    final_position_ = core::FromWorldCoordinates(world_position_) + render_offset_;
    if (auto controlller = dynamic_cast<ugdk::graphic::PrimitiveControllerPosition*>(primitive_.controller().get())) {
        controlller->ChangePosition(final_position_);
    }
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
        animator_->player()->Update(dt);
}

void Graphic::OnAdd(sprite::WorldObject* wobj) {
    SetPosition(wobj->world_position());
}
    
Graphic* Graphic::Create(const std::function<void(ugdk::graphic::Primitive&)>& primitive_prepare_function) {
    Graphic* g = new Graphic(nullptr);
    Primitive& gp = g->primitive();
    primitive_prepare_function(gp);
    if (!gp.shader_program())
        gp.set_shader_program(get_horus_light_shader());
    return g;
}

Graphic* Graphic::Create(const ugdk::graphic::Spritesheet* spritesheet, Animator* animator) {
    using namespace ugdk::graphic;
    
    Graphic* g = new Graphic(animator);
    if (spritesheet) {
        ugdk::graphic::PrimitiveSetup::Sprite::Prepare(g->primitive(), spritesheet);
        dynamic_cast<PrimitiveControllerSprite*>(g->primitive().controller().get())
            ->ChangeToFrame(ugdk::action::SpriteAnimationFrame::DEFAULT()); // guarantee the primitive is in a valid frame.
    }
    if (!g->primitive().shader_program())
        g->primitive().set_shader_program(get_horus_light_shader());
    return g;
}
    
Graphic* Graphic::Create(const std::string& spritesheet_name, const std::string& animation_set) {
    return Create(ugdk::resource::GetSpritesheetFromTag(spritesheet_name), new Animator(animation_set));
}

Graphic* Graphic::Create(const std::string& spritesheet_name) {
    return Create(spritesheet_name.c_str());
}
    
Graphic* Graphic::Create(const char* spritesheet_name) {
    return Create(ugdk::resource::GetSpritesheetFromTag(spritesheet_name), nullptr);
}

}  // namespace component
