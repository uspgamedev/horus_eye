#ifndef HORUSEYE_GAME_SPRITE_HERO_H_
#define HORUSEYE_GAME_SPRITE_HERO_H_

#include <map>
#include <ugdk/math/vector2D.h>

#include "game/components/creature.h"
#include "game/resources/resource.h"
#include "game/resources/capacityblocks.h"

namespace skills {
class Skill;
} // skills

namespace component {

class Hero : public Creature {
  
  public:
    Hero(sprite::WorldObject* owner, 
         ugdk::graphic::Spritesheet* img, 
         resource::Energy &life, 
         resource::Energy &mana, 
         int num_blocks, 
         double mana_per_block);
    ~Hero();

    double FullMana();

    resource::CapacityBlocks& mana_blocks() { return mana_blocks_; }

    void AddWeapon(int slot, skills::Skill* combat_art);
    void StartAttackAnimation();

    skills::Skill* secondary_combat_art() { return active_skills_[Controller::SECONDARY]; }

    void SetupCollision();
    
  private:
    std::map<int, skills::Skill*> skills_;
    int slot_selected_;

    resource::CapacityBlocks mana_blocks_;

    virtual void Update(double delta_t);
    virtual void PlayHitSound() const;
   
    void UpdateAim();
    bool Aiming();
    void ChangeSecondaryWeapon(int slot);
    void CollisionSlow();
    void AddKnownCollisions();

    friend class MummySlowCollision;
};

}
#endif // HORUSEYE_GAME_SPRITE_HERO_H_
