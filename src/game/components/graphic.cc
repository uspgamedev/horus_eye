#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/time/timeaccumulator.h>
#include "game/components/graphic.h"

namespace component {

Graphic::Graphic(sprite::WorldObject* owner)
  : Base("caster"),
    node_(new ugdk::graphic::Node),
    owner_(owner),
    is_blinking_(false),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false) {}

Graphic::~Graphic() {
    delete node_;
    delete blink_time_;
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

void Graphic::AdjustBlink() {
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
