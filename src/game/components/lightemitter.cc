#include "game/components/lightemitter.h"

#include "game/constants.h"
#include "game/core/coordinates.h"
#include "game/initializer.h"
#include "game/core/world.h"
#include "game/sprites/worldobject.h"

#include <ugdk/graphic/light.h>
#include <ugdk/graphic/canvas.h>

#define LIGHT_COEFFICIENT 1.0

namespace component {

using ugdk::math::Vector2D;

LightEmitter::LightEmitter(double light_radius, ugdk::structure::Color color)
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

void LightEmitter::ChangeColor(const ugdk::structure::Color& color) {
    color_ = color;
    if(light_)
        light_->set_color(color_);
}

void LightEmitter::Update(double dt) {}

void LightEmitter::Render(ugdk::graphic::Canvas& canvas) const {
    canvas.PushAndCompose(ugdk::math::Geometry(position_));
    light_->Draw(canvas);
    canvas.PopGeometry();
}
    
void LightEmitter::OnAdd(sprite::WObjRawPtr wobj) {
    SetPosition(wobj->world_position());
}


}  // namespace component
