#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/time/timeaccumulator.h>
#include "damageable.h"
#include "game/sprites/worldobject.h"
#include "game/utils/settings.h"

using namespace ugdk;

using sprite::WorldObject;

#define PI 3.1415926535897932384626433832795

namespace component {

Damageable::Damageable(sprite::WorldObject* owner)
  : owner_(owner),
    super_armor_(false),
    invulnerability_time_(0),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    hit_duration_(new ugdk::time::TimeAccumulator(0)) {
        for(int i = 1; i <= 4; ++i) {
            char buffer[255];
            sprintf(buffer, "samples/hit%d.wav", i);
            hit_sounds_.push_back(buffer);
        }
}
Damageable::~Damageable() {
    if (hit_duration_) delete hit_duration_;
    if (blink_time_) delete blink_time_;
}

void Damageable::TakeDamage(double life_points) {
    if(!hit_duration_->Expired()) return;
#ifdef DEBUG
    int creature_id = static_cast<int>(reinterpret_cast<uintptr_t>(this) & 0xFFFFFF);
    fprintf(stderr, "Damage to %s [%X]. DMG: %.2f; Life: %.2f -> %.2f\n", owner_->identifier().c_str(), creature_id,
        life_points, (double) life_, (double) life_ - life_points);
#endif
    PlayHitSound();
    life_ -= life_points;
    if(life_.Empty()) {
        if (owner_->is_active()) {
            //sprite_->SelectAnimation(dying_animation_);
	        owner_->StartToDie();
#ifdef DEBUG
            fprintf(stderr, "\tTriggering death animation.\n");
#endif
        }
    } else if(!super_armor_) {
        //waiting_animation_ = true;
        //sprite_->SelectAnimation(taking_damage_animation_);
    }
    hit_duration_->Restart(invulnerability_time_);
    blink_time_->Restart();
}

void Damageable::PlayHitSound() const {
    if(hit_sounds_.empty()) return;
    if(utils::Settings::reference()->sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample(hit_sounds_[rand() % hit_sounds_.size()])->Play();
}


}  // namespace sprite
