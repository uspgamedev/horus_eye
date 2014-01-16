#include "game/components/light.h"

#include "game/constants.h"
#include "game/core/coordinates.h"
#include "game/initializer.h"
#include "game/scenes/world.h"

#include <ugdk/graphic/light.h>
#include <ugdk/graphic/canvas.h>

#define LIGHT_COEFFICIENT 0.75

namespace component {

using ugdk::math::Vector2D;

Light::Light(double light_radius)
    :   light_(nullptr)
    ,   radius_(light_radius)
{
    if(radius_ > 0)
        ChangeRadius(radius_);
}

Light::~Light() {
    delete light_;
}

void Light::SetPosition(const ugdk::math::Vector2D& position) {
    position_ = core::FromWorldCoordinates(position);
}

void Light::ChangeRadius(double radius) {
    radius_ = radius;
    
    if(radius_ > constants::GetDouble("LIGHT_RADIUS_THRESHOLD")) {
        if(!light_) {
            light_ = new ugdk::graphic::Light;
            light_->set_color(color_);
        }

        Vector2D dimension = core::ConvertLightRadius(radius_);
        light_->set_dimension(dimension * LIGHT_COEFFICIENT);

    } else {
        delete light_;
        light_ = nullptr;
    }
}

void Light::ChangeColor(const ugdk::Color& color) {
    color_ = color;
    if(light_)
        light_->set_color(color_);
}

void Light::Update(double dt) {}

void Light::Render(ugdk::graphic::Canvas& canvas) const {
    light_->Draw(canvas.current_geometry() * ugdk::graphic::Geometry(position_));
}

}  // namespace component
