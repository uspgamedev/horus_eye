#ifndef HORUSEYE_COMPONENT_DAMAGEABLE_H_
#define HORUSEYE_COMPONENT_DAMAGEABLE_H_

#include <vector>
#include <string>
#include <ugdk/time.h>

#include "game/sprites.h"
#include "game/resources/energy.h"

namespace component {

class Damageable { 
  public:
    Damageable(sprite::WorldObject* owner);
    virtual ~Damageable();

    sprite::WorldObject* owner() { return owner_; }

    virtual void TakeDamage(double life_points);

    resource::Energy& life() { return life_; }
	void set_life(resource::Energy &life) {
		life_ = life;
	}
	void set_life(double life) {
	    life_.Set(life);
	}

    void set_super_armor(bool super_armor) { super_armor_ = super_armor; }

    void add_hit_sound(const std::string& sound) {
        hit_sounds_.push_back(sound);
    }

  protected:
    void PlayHitSound() const;

    /// The owner.
    sprite::WorldObject* owner_;

    /// The life part of the component. An energy manages reneration.
    resource::Energy life_;

    /// When true, TakeDamage won't cause flinch.
    bool super_armor_;

    /// Duration of the invulnerability after taking a hit.
    int invulnerability_time_;

    /// Controls when to toggle the blink_ flag.
    ugdk::time::TimeAccumulator *blink_time_;

    /// Controls the invulnerability after being hit.
    ugdk::time::TimeAccumulator *hit_duration_;

    /// List of possible sound effects to play when hit.
    std::vector<std::string> hit_sounds_;

};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_
