#include "game/components/basegraphic.h"

#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/time/timeaccumulator.h>
#include <ugdk/math/vector2D.h>

#include "game/components/orders.h"
#include "game/constants.h"
#include "game/scenes/world.h"

#define LIGHT_COEFFICIENT 0.75

namespace component {

using ugdk::Vector2D;
using ugdk::graphic::Node;
using ugdk::graphic::Drawable;

BaseGraphic::BaseGraphic()
  : node_(new Node),
    layer_(scene::FOREGROUND_LAYER),
    is_blinking_(false),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(0.0) {}

BaseGraphic::BaseGraphic(ugdk::graphic::Drawable* drawable)
  : node_(new Node(drawable)),
    layer_(scene::FOREGROUND_LAYER),
    is_blinking_(false),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(0.0) {}

BaseGraphic::BaseGraphic(ugdk::graphic::Drawable* drawable, double light_radius)
  : node_(new Node(drawable)),
    layer_(scene::FOREGROUND_LAYER),
    is_blinking_(false),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(light_radius) {
        ChangeLightRadius(light_radius_);
}

BaseGraphic::~BaseGraphic() {
    delete node_;
    delete blink_time_;
    delete blink_duration_;
}

void BaseGraphic::SetPosition(const ugdk::Vector2D& position) {
    Vector2D screen_position = scene::World::FromWorldCoordinates(position);
    node_->modifier()->set_offset(screen_position + render_offset_);
    node_->set_zindex(screen_position.y);
}

void BaseGraphic::ChangeLightRadius(double radius) {
    light_radius_ = radius;
    
    if(light_radius_ > constants::GetDouble("LIGHT_RADIUS_THRESHOLD")) {
        if(node_->light() == NULL) {
            node_->set_light(new ugdk::graphic::Light);
            node_->light()->set_color(light_color_);
        }

        Vector2D dimension = scene::World::ConvertLightRadius(light_radius_);
        node_->light()->set_dimension(dimension * LIGHT_COEFFICIENT);

    } else {
        if(node_->light()) {
            delete node_->light();
            node_->set_light(NULL);
        }
    }
}

void BaseGraphic::ChangeLightColor(const ugdk::Color& color) {
    light_color_ = color;
    if(node_->light())
        node_->light()->set_color(light_color_);
}

double BaseGraphic::alpha() const {
    return node_->modifier()->color().a;
}

void BaseGraphic::ChangeAlpha(double alpha) {
    ugdk::Color color = node_->modifier()->color();
    color.a = alpha;
    node_->modifier()->set_color(color);
}

void BaseGraphic::set_visible(bool visible) {
    node_->modifier()->set_visible(visible);
}

void BaseGraphic::StartBlinking(int duration) {
    is_blinking_ = true;
    blink_time_->Restart();
    if(duration <= 0)
        blink_duration_->Pause();
    else
        blink_duration_->Restart(duration);
}

void BaseGraphic::StopBlinking() {
    is_blinking_ = false;
    ugdk::Color c = node_->modifier()->color();
    c.a = 1.0;
    node_->modifier()->set_color(c);
}

void BaseGraphic::InsertIntoLayers(ugdk::graphic::Node** layers) {
    layers[0]->AddChild(node_);
    //layer_node((*it)->layer())->AddChild((*it)->node());
}

void BaseGraphic::RemoveFromLayers(ugdk::graphic::Node** layers) {
    layers[0]->RemoveChild(node_);
}

void BaseGraphic::adjustBlink() {
    if(is_blinking_ && !blink_duration_->IsPaused() && blink_duration_->Expired())
        StopBlinking();
    if (is_blinking_ && blink_time_->Expired()) {
        blink_ = !blink_;
        ugdk::Color c = node_->modifier()->color();
        c.a = blink_ ? 1.0 : 0.20;
        node_->modifier()->set_color(c);
        blink_time_->Restart();
    }
}

}  // namespace component
