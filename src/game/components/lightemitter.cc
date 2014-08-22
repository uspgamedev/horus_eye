#include "game/components/lightemitter.h"

#include "game/constants.h"
#include "game/core/coordinates.h"
#include "game/initializer.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"

#include <ugdk/graphic/light.h>
#include <ugdk/graphic/canvas.h>

#define LIGHT_COEFFICIENT 7.5

namespace component {

using ugdk::math::Vector2D;

LightEmitter::LightEmitter(double light_radius, ugdk::Color color)
    :   light_(nullptr)
    ,   radius_(light_radius)
    ,   color_(color)
{
    if(radius_ > 0)
        ChangeRadius(radius_);
}

LightEmitter::~LightEmitter() {
    delete light_;
}

void LightEmitter::SetPosition(const ugdk::math::Vector2D& position) {
    position_ = position;
}

void LightEmitter::ChangeRadius(double radius) {
    radius_ = radius;
    
    if(radius_ > constants::GetDouble("LIGHT_RADIUS_THRESHOLD")) {
        if(!light_) {
            light_ = new ugdk::graphic::Light;
            light_->set_color(color_);
        }

        Vector2D dimension(radius_, radius_);
        light_->set_dimension(dimension * LIGHT_COEFFICIENT);

    } else {
        delete light_;
        light_ = nullptr;
    }
}

void LightEmitter::ChangeColor(const ugdk::Color& color) {
    color_ = color;
    if(light_)
        light_->set_color(color_);
}

void LightEmitter::Update(double dt) {}

void LightEmitter::Render(ugdk::graphic::Canvas& canvas) const {
    light_->Draw(ugdk::graphic::Geometry(Vector2D(), Vector2D(1.0 / canvas.size().x, 1.0 / canvas.size().y)) *
                 ugdk::graphic::Geometry(position_));
}
    
void LightEmitter::OnAdd(sprite::WorldObject* wobj) {
    SetPosition(wobj->world_position());
}


}  // namespace component
