#ifndef HORUSEYE_GAME_SPRITE_HERO_H_
#define HORUSEYE_GAME_SPRITE_HERO_H_

#include <map>
#include <ugdk/math/vector2D.h>
#include "game/sprites/creatures/creature.h"

namespace ugdk {
class Image;
}

namespace sprite {

class Hero : public Creature {
  DEFINE_COLLIDABLE
  public:
    Hero(ugdk::Image* img = NULL);
    ~Hero();

    void AddWeapon(int slot, Weapon* weapon);
    void StartAttack();
    void StartExplosion();
    Weapon* secondary_weapon() { return secondary_weapon_; }

	void Invulnerable(int time);

  private:
    ugdk::Vector2D screen_center_;
    bool pressed_key_[4];
    float time_to_recover_speed_;
    std::map<int, Weapon*> weapons_;
    int slot_selected_;
    float light_oscilation_;
    Weapon *secondary_weapon_;

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
