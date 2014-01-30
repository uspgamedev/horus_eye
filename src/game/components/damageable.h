#ifndef HORUSEYE_COMPONENT_DAMAGEABLE_H_
#define HORUSEYE_COMPONENT_DAMAGEABLE_H_

#include "game/components/base.h"

#include "game/sprites.h"
#include "game/resources/energy.h"

#include <ugdk/time.h>
#include <ugdk/system/compatibility.h>

#include <vector>
#include <string>

namespace component {

class Damageable : public Base { 
  public:
    static const char* DEFAULT_NAME() { return "damageable"; }
    static int DEFAULT_ORDER() { return 0; }

    Damageable(int invulnerability_time = 0, bool blinks = false);
    virtual ~Damageable();

    virtual void TakeDamage(double life_points);

    void Update(double dt) override;
    void OnAdd(sprite::WorldObject*) override;

    resource::Energy& life() { return life_; }
    void set_life(const resource::Energy &life) {
        life_ = life;
    }
    void set_life(double life) {
        life_.Set(life);
    }

    void set_super_armor(bool super_armor) { super_armor_ = super_armor; }

    void AddHitSound(const std::string& sound) {
        hit_sounds_.push_back(sound);
    }

    /// True if this damageable is currently invincible for taking a hit.
    bool IsMercyInvincible() const;
    bool blinks() const { return blink_time_ != nullptr; }

  protected:
    void PlayHitSound() const;
    void StopBlinking();

    /// The owner.
    sprite::WorldObject* owner_;

    /// The life part of the component. An energy manages reneration.
    resource::Energy life_;

    /// When true, TakeDamage won't cause flinch.
    bool super_armor_;

    /// Duration of the invulnerability after taking a hit.
    int invulnerability_time_;

    /// Controls the invulnerability after being hit.
    ugdk::time::TimeAccumulator *hit_duration_;

    /// List of possible sound effects to play when hit.
    std::vector<std::string> hit_sounds_;

    /// Controls when to toggle the blink_ flag. Also controls if it should blink at all.
    ugdk::time::TimeAccumulator *blink_time_;

};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_
