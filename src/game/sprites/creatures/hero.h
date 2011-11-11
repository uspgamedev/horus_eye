#ifndef HORUSEYE_GAME_SPRITE_HERO_H_
#define HORUSEYE_GAME_SPRITE_HERO_H_

#include <map>
#include <ugdk/math/vector2D.h>
#include "game/sprites/creatures/creature.h"

namespace skills {
class AbstractSkill;

namespace castarguments {
class Aim;
} // castarguments

} // skills

namespace ugdk {
class Image;
}

namespace sprite {

class Hero : public Creature {
  
  public:
    Hero(ugdk::Image* img = NULL);
    ~Hero();

    skills::castarguments::Aim* aim() { return aim_; }

    void AddWeapon(int slot, skills::AbstractSkill* combat_art);
    void StartAttack();
    void StartExplosion();
    skills::AbstractSkill* secondary_combat_art() { return secondary_weapon_; }

	void Invulnerable(int time);

  private:
    ugdk::Vector2D screen_center_;
    bool pressed_key_[4];
    float time_to_recover_speed_;
    std::map<int, skills::AbstractSkill*> weapons_;
    int slot_selected_;
    float light_oscilation_;
    skills::AbstractSkill *secondary_weapon_;

    skills::castarguments::Aim* aim_;


    void CollisionSlow();
	COLLISION_BEGIN
        COLLISION_ADD_INLINE (Hero, MummySlow, owner_->CollisionSlow(); )
	COLLISION_END

    virtual void Update(float delta_t);
    virtual void PlayHitSound() const;
    
    bool ShootingWithWeapon();
    bool ShootingWithSecondaryWeapon();
    void GetKeys();
    void ChangeSecondaryWeapon(int slot);
};

}
#endif // HORUSEYE_GAME_SPRITE_HERO_H_
