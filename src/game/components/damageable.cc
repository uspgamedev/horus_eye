#include "damageable.h"

#include "game/components/animation.h"
#include "game/components/body.h"
#include "game/components/graphic.h"
#include "game/sprites/worldobject.h"
#include "communication/direct.h"

#include <ugdk/debug/log.h>
#include <ugdk/system/engine.h>
#include <ugdk/audio/module.h>
#include <ugdk/time/timeaccumulator.h>

using namespace ugdk;

using sprite::WorldObject;

#define PI 3.1415926535897932384626433832795

namespace component {

static const int BLINKING_INTERVAL = 75;

Damageable::Damageable(int invulnerability_time, bool blinks)
    : owner_(nullptr)
    , super_armor_(false)
    , invulnerability_time_(invulnerability_time)
    , hit_duration_(new ugdk::time::TimeAccumulator(0))
    , blink_time_(blinks ? new ugdk::time::TimeAccumulator(BLINKING_INTERVAL) : nullptr)
    , dead_(false)
{}

Damageable::~Damageable() {
    delete hit_duration_;
    delete blink_time_;
}

void Damageable::TakeDamage(double life_points) {
    if(IsMercyInvincible()) return;
    //#ifdef DEBUG
    //    fprintf(stderr, "Damage to %s [%p]. DMG: %.2f; Life: %.2f -> %.2f\n", owner_->identifier().c_str(), this,
    //        life_points, (double) life_, (double) life_ - life_points);
    //#endif
    PlayHitSound();
    life_ -= life_points;
    component::Animation* animation = owner_->component<component::Animation>();
    if(life_.Empty()) {
        Die();
    } else if(!super_armor_ && animation) {
        animation->ChangeAnimation(utils::TAKING_HIT);
    }

    // Start mercy invincibility
    hit_duration_->Restart(invulnerability_time_);

    // Start blinking!
    if(invulnerability_time_ > 0 && blink_time_)
        blink_time_->Restart();
}

void Damageable::Die() {
    if (dead_) return;
    dead_ = true;
    
    if (auto animation = owner_->animation())
        animation->ChangeAnimation(utils::DEATH);
    else
        owner_->Remove();

    if (auto body = owner_->body())
        body->Deactivate();

    for (const auto& callback : on_die_callbacks_)
        callback(owner_);
}

void Damageable::Update(double dt) {
    life_.Update(dt);

    if(blinks() && IsMercyInvincible() && blink_time_->Expired()) {
        if(auto g = owner_->graphic())
            g->set_visible(!g->visible());
        blink_time_->Restart();
    }
    if(hit_duration_->Expired())
        if(auto g = owner_->graphic())
            g->set_visible(true);
}
    
void Damageable::OnAdd(sprite::WObjRawPtr wobj) {
    owner_ = wobj;
}
    
bool Damageable::IsMercyInvincible() const {
    return !hit_duration_->Expired();
}

void Damageable::PlayHitSound() const {
    if(hit_sounds_.empty()) return;
    communication::notify::PlaySound(hit_sounds_[rand() % hit_sounds_.size()]);
}


}  // namespace sprite
