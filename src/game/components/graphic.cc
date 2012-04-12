#include <ugdk/graphic/node.h>
#include <ugdk/time/timeaccumulator.h>
#include "game/components/graphic.h"

namespace component {

Graphic::Graphic(sprite::WorldObject* owner)
  : BaseComponent(owner),
    node_(new ugdk::graphic::Node),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    is_blinking_(false),
    blink_(false) {}

Graphic::~Graphic() {
    delete node_;
    delete blink_time_;
}

void Graphic::StartBlinking() {
    is_blinking_ = true;
    blink_time_->Restart();
}

void Graphic::StopBlinking() {
    is_blinking_ = false;
    node()->modifier()->set_alpha(1.0);
}

void Graphic::AdjustBlink() {
    if (is_blinking_ && blink_time_->Expired()) {
        blink_ = !blink_;
        node()->modifier()->set_alpha(blink_ ? 1.0 : 0.20);
        blink_time_->Restart();
    }
}

}  // namespace component
