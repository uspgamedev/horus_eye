#include "game/components/graphic.h"

#include <glm/glm.hpp>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/sprite.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/resource/module.h>

#include "game/core/coordinates.h"
#include "game/components/animator.h"
#include "game/sprites/worldobject.h"
#include "game/initializer.h"

#include "game/map/specialwall.h"
#include "frontend/gameview/objectgraphic.h"

namespace component {

using ugdk::math::Vector2D;
using ugdk::graphic::Primitive;

Graphic::Graphic(frontend::gameview::ObjectGraphic* g)
    : frontend_graphic_(g)
{}

Graphic::~Graphic() {}

void Graphic::set_layer(core::GameLayer layer) {
    frontend_graphic_->set_layer(layer);
}
        
void Graphic::SetPosition(const ugdk::math::Vector2D& position) {
    frontend_graphic_->SetPosition(position);
}

ugdk::math::Vector2D Graphic::render_offset() const {
    return frontend_graphic_->render_offset();
}

void Graphic::set_render_offset(const ugdk::math::Vector2D& render_offset) {
    frontend_graphic_->set_render_offset(render_offset);
}
    
double Graphic::alpha() const {
    return frontend_graphic_->visual_effect().color().a;
}

void Graphic::ChangeAlpha(double alpha) {
    frontend_graphic_->ChangeAlpha(alpha);
}

bool Graphic::visible() const {
    return frontend_graphic_->visible();
}

void Graphic::set_visible(bool visible) {
    frontend_graphic_->set_visible(visible);
}

void Graphic::Update(double dt) {}

void Graphic::OnAdd(sprite::WObjRawPtr wobj) {
    SetPosition(wobj->world_position());
    if (auto animator = wobj->animator())
        animator->Configure(this);
}

void Graphic::OnObjectRemoved() {
    set_visible(false);
}
    
void Graphic::ChangeToFrame(const std::string& frame_name) {
    frontend_graphic_->ChangeToFrame(frame_name);
}

void Graphic::ChangeToFrame(std::size_t frame_number) {
    frontend_graphic_->ChangeToFrame(frame_number);
}

void Graphic::ChangeToAnimationFrame(const ugdk::graphic::SpriteAnimationFrame& frame) {
    frontend_graphic_->ChangeToAnimationFrame(frame);
}

Graphic* Graphic::Create(CreateTypes type, const std::string& arg) {
    return new Graphic(frontend::gameview::ObjectGraphic::Create(type, arg));
}

Graphic* Graphic::CreateWithSpritesheet(const std::string& spritesheet_name) {
    return new Graphic(frontend::gameview::ObjectGraphic::CreateWithSpritesheet(spritesheet_name));
}

Graphic* Graphic::CreateWithSingleFrame(const std::string& spritesheet_name, const std::string& frame_name) {
    return new Graphic(frontend::gameview::ObjectGraphic::CreateWithSingleFrame(spritesheet_name, frame_name));
}
    
}  // namespace component
