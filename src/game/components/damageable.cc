#include "damageable.h"

#include "game/components/animation.h"
#include "game/components/graphic.h"
#include "game/sprites/worldobject.h"
#include "game/utils/settings.h"

#include <ugdk/system/engine.h>
#include <ugdk/audio/module.h>
#include <ugdk/time/timeaccumulator.h>

using namespace ugdk;

using sprite::WorldObject;

#define PI 3.1415926535897932384626433832795

namespace component {

static const int BLINKING_INTERVAL = 75;

Damageable::Damageable(sprite::WorldObject* owner, int invulnerability_time, bool blinks)
    :   owner_(owner)
    ,   super_armor_(false)
    ,   invulnerability_time_(invulnerability_time)
    ,   hit_duration_(new ugdk::time::TimeAccumulator(0))
    ,   blink_time_(blinks ? new ugdk::time::TimeAccumulator(BLINKING_INTERVAL) : nullptr)
{}

Damageable::~Damageable() {
    delete hit_duration_;
    delete blink_time_;
}

void Damageable::TakeDamage(double life_points) {
    if(IsMercyInvincible()) return;
    #ifdef DEBUG
        fprintf(stderr, "Damage to %s [%p]. DMG: %.2f; Life: %.2f -> %.2f\n", owner_->identifier().c_str(), this,
            life_points, (double) life_, (double) life_ - life_points);
    #endif
    PlayHitSound();
    life_ -= life_points;
    component::Animation* animation = owner_->component<component::Animation>();
    if(life_.Empty()) {
        if (!owner_->dead()) {
            if (animation)
                animation->ChangeAnimation(utils::DEATH);
            owner_->Die();
            #ifdef DEBUG
                fprintf(stderr, "\tTriggering death animation.\n");
            #endif
        }
    } else if(!super_armor_ && animation) {
        animation->ChangeAnimation(utils::TAKING_HIT);
    }

    // Start mercy invincibility
    hit_duration_->Restart(invulnerability_time_);

    // Start blinking!
    if(invulnerability_time_ > 0 && blink_time_)
        blink_time_->Restart();
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
    
bool Damageable::IsMercyInvincible() const {
    return !hit_duration_->Expired();
}

void Damageable::PlayHitSound() const {
    if(hit_sounds_.empty()) return;
    if(utils::Settings::reference()->sound_effects())
        ugdk::audio::manager()->LoadSample(hit_sounds_[rand() % hit_sounds_.size()])->Play();
}


}  // namespace sprite
