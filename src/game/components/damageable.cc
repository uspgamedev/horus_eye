#include <ugdk/system/engine.h>
#include <ugdk/audio/module.h>
#include <ugdk/time/timeaccumulator.h>
#include "damageable.h"
#include "game/components/animation.h"
#include "game/components/graphic.h"
#include "game/sprites/worldobject.h"
#include "game/utils/settings.h"

using namespace ugdk;

using sprite::WorldObject;

#define PI 3.1415926535897932384626433832795

namespace component {

Damageable::Damageable(sprite::WorldObject* owner, int invulnerability_time, bool blinks)
  : owner_(owner),
    super_armor_(false),
    invulnerability_time_(invulnerability_time),
    hit_duration_(new ugdk::time::TimeAccumulator(invulnerability_time)),
    blinks_(blinks) {
        for(int i = 1; i <= 4; ++i) {
            char buffer[255];
            sprintf(buffer, "samples/hit%d.wav", i);
            hit_sounds_.push_back(buffer);
        }
}
Damageable::~Damageable() {
    delete hit_duration_;
}

void Damageable::TakeDamage(double life_points) {
    if(!hit_duration_->Expired()) return;
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
            owner_->StartToDie();
            #ifdef DEBUG
                fprintf(stderr, "\tTriggering death animation.\n");
            #endif
        }
    } else if(!super_armor_ && animation) {
        animation->ChangeAnimation(utils::TAKING_HIT);
    }
    hit_duration_->Restart();
    if(invulnerability_time_ > 0 && blinks_)
        owner_->graphic()->StartBlinking(invulnerability_time_);
}

void Damageable::PlayHitSound() const {
    if(hit_sounds_.empty()) return;
    if(utils::Settings::reference()->sound_effects())
        ugdk::audio::manager()->LoadSample(hit_sounds_[rand() % hit_sounds_.size()])->Play();
}


}  // namespace sprite
