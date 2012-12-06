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
  : root_node_(new Node),
    node_(new Node),
    layer_(scene::FOREGROUND_LAYER),
    is_blinking_(false),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(0.0) { root_node_->AddChild(node_); }

BaseGraphic::BaseGraphic(ugdk::graphic::Drawable* drawable)
  : root_node_(new Node),
    node_(new Node(drawable)),
    layer_(scene::FOREGROUND_LAYER),
    is_blinking_(false),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(0.0) { root_node_->AddChild(node_); }

BaseGraphic::BaseGraphic(ugdk::graphic::Drawable* drawable, double light_radius)
  : root_node_(new Node),
    node_(new Node(drawable)),
    layer_(scene::FOREGROUND_LAYER),
    is_blinking_(false),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(light_radius) {
        root_node_->AddChild(node_);
        ChangeLightRadius(light_radius_);
}

BaseGraphic::~BaseGraphic() {
    delete root_node_;
    delete blink_time_;
    delete blink_duration_;
}

void BaseGraphic::SetPosition(const ugdk::Vector2D& position) {
    Vector2D screen_position = scene::World::FromWorldCoordinates(position);
    root_node_->modifier()->set_offset(screen_position);
    root_node_->set_zindex(screen_position.y);
}

void BaseGraphic::set_render_offset(const ugdk::Vector2D& render_offset) {
    render_offset_ = render_offset;
    node_->modifier()->set_offset(render_offset_);
}

void BaseGraphic::ChangeLightRadius(double radius) {
    light_radius_ = radius;
    
    if(light_radius_ > constants::GetDouble("LIGHT_RADIUS_THRESHOLD")) {
        if(root_node_->light() == NULL) {
            root_node_->set_light(new ugdk::graphic::Light);
            root_node_->light()->set_color(light_color_);
        }

        Vector2D dimension = scene::World::ConvertLightRadius(light_radius_);
        root_node_->light()->set_dimension(dimension * LIGHT_COEFFICIENT);

    } else {
        if(root_node_->light()) {
            delete root_node_->light();
            root_node_->set_light(NULL);
        }
    }
}

void BaseGraphic::ChangeLightColor(const ugdk::Color& color) {
    light_color_ = color;
    if(root_node_->light())
        root_node_->light()->set_color(light_color_);
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
    root_node_->modifier()->set_visible(visible);
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
    node_->modifier()->set_visible(true);
}

void BaseGraphic::InsertIntoLayers(ugdk::graphic::Node** layers) {
    layers[layer_]->AddChild(root_node_);
}

void BaseGraphic::RemoveFromLayers(ugdk::graphic::Node** layers) {
    layers[layer_]->RemoveChild(root_node_);
}

void BaseGraphic::adjustBlink() {
    if(is_blinking_ && !blink_duration_->IsPaused() && blink_duration_->Expired())
        StopBlinking();
    if (is_blinking_ && blink_time_->Expired()) {
        blink_ = !blink_;
        node_->modifier()->set_visible(blink_);
        blink_time_->Restart();
    }
}

}  // namespace component
