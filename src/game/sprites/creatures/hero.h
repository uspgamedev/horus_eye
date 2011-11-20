#ifndef HORUSEYE_GAME_SPRITE_HERO_H_
#define HORUSEYE_GAME_SPRITE_HERO_H_

#include <map>
#include <ugdk/math/vector2D.h>

#include "game/sprites/creatures/creature.h"
#include <game/resources/resource.h>
#include "game/resources/capacityblocks.h"

namespace skills {
class Skill;
} // skills

namespace ugdk {
class Image;
} // ugdk

namespace sprite {

class Hero : public Creature {
  
  public:
    Hero(ugdk::Image* img, 
         resource::Energy &life, 
         resource::Energy &mana, 
         int num_blocks, 
         float mana_per_block);
    ~Hero();

    float FullMana();

    resource::CapacityBlocks& mana_blocks() { return mana_blocks_; }

    void AddWeapon(int slot, skills::Skill* combat_art);
    void StartAttackAnimation();

    skills::Skill* secondary_combat_art() { return secondary_weapon_; }

	void Invulnerable(int time);
    
  private:
    bool pressed_key_[4];
    std::map<int, skills::Skill*> weapons_;
    skills::Skill *secondary_weapon_;
    int slot_selected_;
    float light_oscilation_;

    resource::CapacityBlocks mana_blocks_;

    virtual void Update(float delta_t);
    virtual void PlayHitSound() const;
   
    void UpdateAim();
    bool ShootingWithWeapon();
    bool ShootingWithSecondaryWeapon();
    void GetKeys();
    void ChangeSecondaryWeapon(int slot);
    void CollisionSlow();

    friend class MummySlowCollision;
};

}
#endif // HORUSEYE_GAME_SPRITE_HERO_H_
