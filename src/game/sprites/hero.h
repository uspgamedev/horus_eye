#ifndef HORUSEYE_GAME_SPRITE_HERO_H_
#define HORUSEYE_GAME_SPRITE_HERO_H_

#include "creature.h"
#include "../../framework/image.h"
#include "../../framework/vector2D.h"
#include <map>

namespace sprite {

class Mummy;
class MummyProjectile;
class Hero : public Creature {

  public:
    Hero(ugdk::Image* img = NULL);
    ~Hero() {}

    virtual void CollidesWith(Mummy *);
    virtual void CollidesWith(MummyProjectile *);
    virtual void HandleCollision(WorldObject *);
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
    Weapon *secondary_weapon_;

    virtual void Update(float delta_t);
    virtual void PlayHitSound() const;
    
    bool ShootingWithWeapon();
    bool ShootingWithSecondaryWeapon();
    void GetKeys();
    void ChangeSecondaryWeapon(int slot);
};

}
#endif // HORUSEYE_GAME_SPRITE_HERO_H_
