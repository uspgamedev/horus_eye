#include "game/components/graphic.h"

#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/time/timeaccumulator.h>

#include "game/components/orders.h"
#include "game/constants.h"
#include "game/scenes/world.h"

#define LIGHT_COEFFICIENT 0.75

namespace component {

using ugdk::Vector2D;
using ugdk::graphic::Node;
using ugdk::graphic::Drawable;

Graphic::Graphic()
  : node_(new Node),
    layer_(scene::FOREGROUND_LAYER),
    is_blinking_(false),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(0.0) {}

Graphic::Graphic(ugdk::graphic::Drawable* drawable)
  : node_(new Node(drawable)),
    layer_(scene::FOREGROUND_LAYER),
    is_blinking_(false),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(0.0) {}

Graphic::Graphic(ugdk::graphic::Drawable* drawable, double light_radius)
  : node_(new Node(drawable)),
    layer_(scene::FOREGROUND_LAYER),
    is_blinking_(false),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(light_radius) {
        ChangeLightRadius(light_radius_);
}

Graphic::~Graphic() {
    delete node_;
    delete blink_time_;
}

void Graphic::ChangeLightRadius(double radius) {
    light_radius_ = radius;
    
    if(light_radius_ > constants::GetDouble("LIGHT_RADIUS_THRESHOLD")) {
        if(node_->light() == NULL) 
            node_->set_light(new ugdk::graphic::Light);

        Vector2D dimension = scene::World::ConvertLightRadius(light_radius_);
        node_->light()->set_dimension(dimension * LIGHT_COEFFICIENT);

    } else {
        if(node_->light()) {
            delete node_->light();
            node_->set_light(NULL);
        }
    }
}

void Graphic::StartBlinking(int duration) {
    is_blinking_ = true;
    blink_time_->Restart();
    if(duration <= 0)
        blink_duration_->Pause();
    else
        blink_duration_->Restart(duration);
}

void Graphic::StopBlinking() {
    is_blinking_ = false;
    ugdk::Color c = node()->modifier()->color();
    c.a = 1.0;
    node()->modifier()->set_color(c);
}

void Graphic::InsertIntoLayers(ugdk::graphic::Node** layers) {
    layers[0]->AddChild(node());
    //layer_node((*it)->layer())->AddChild((*it)->node());
}

void Graphic::RemoveFromLayers(ugdk::graphic::Node** layers) {
    layers[0]->RemoveChild(node());
}

void Graphic::adjustBlink() {
    if(is_blinking_ && !blink_duration_->IsPaused() && blink_duration_->Expired())
        StopBlinking();
    if (is_blinking_ && blink_time_->Expired()) {
        blink_ = !blink_;
        ugdk::Color c = node()->modifier()->color();
        c.a = blink_ ? 1.0 : 0.20;
        node()->modifier()->set_color(c);
        blink_time_->Restart();
    }
}

}  // namespace component
