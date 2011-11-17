#ifndef HORUSEYE_GAME_SPRITE_HERO_H_
#define HORUSEYE_GAME_SPRITE_HERO_H_

#include <map>
#include <ugdk/math/vector2D.h>

#include "game/sprites/creatures/creature.h"
#include <game/resources/resource.h>
#include <game/resources/countableresource.h>

namespace skills {
class Skill;
} // skills

namespace ugdk {
class Image;
} // ugdk

namespace sprite {

class Hero : public Creature {
  
  public:
    Hero(ugdk::Image* img = NULL);
    ~Hero();

    float FullMana();
    bool HasBreakableManaBlocks(int quantity);
    void BreakManaBlocks(int quantity);
    void RepairManaBlocks(int quantity);
    resource::Resource<int>& mana_regen_ratio() { return mana_regen_ratio_; }

    void AddWeapon(int slot, skills::Skill* combat_art);
    void StartAttackAnimation();

    void StartExplosion();
    skills::Skill* secondary_combat_art() { return secondary_weapon_; }

	void Invulnerable(int time);

  private:
    ugdk::Vector2D screen_center_;
    bool pressed_key_[4];
    float time_to_recover_speed_;
    std::map<int, skills::Skill*> weapons_;
    int slot_selected_;
    float light_oscilation_;
    skills::Skill *secondary_weapon_;

    resource::CountableResource mana_blocks_;
    resource::Resource<int> mana_regen_ratio_;

    void CollisionSlow();
	COLLISION_BEGIN
        COLLISION_ADD_INLINE (Hero, MummySlow, owner_->CollisionSlow(); )
	COLLISION_END

    virtual void Update(float delta_t);
    virtual void PlayHitSound() const;
   
    void UpdateAim();
    bool ShootingWithWeapon();
    bool ShootingWithSecondaryWeapon();
    void GetKeys();
    void ChangeSecondaryWeapon(int slot);
};

}
#endif // HORUSEYE_GAME_SPRITE_HERO_H_
