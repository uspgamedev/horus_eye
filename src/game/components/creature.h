#ifndef HORUSEYE_COMPONENT_CREATURE_H_
#define HORUSEYE_COMPONENT_CREATURE_H_

#include <list>
#include <map>

#include <ugdk/action.h>
#include <ugdk/time.h>
#include <ugdk/graphic.h>

#include <ugdk/action/observer.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>
#include <pyramidworks/geometry.h>
#include "game/components/logic.h"
#include "game/components/controller.h"
#include "game/sprites/worldobject.h"
#include <game/resources/energy.h>
#include "game/skills/usearguments.h"

namespace skills {
    class Skill;
}

namespace sprite {
    class Condition;
}

namespace component {
    
class Creature : public Logic { 
  public:
    Creature(sprite::WorldObject* owner, Controller* controller);
    virtual ~Creature();

    sprite::WorldObject* owner() { return owner_; }

    resource::Energy& mana() { return mana_; }
    void set_mana(resource::Energy &mana) { mana_ = mana; }
    void set_mana(double mana) { mana_.Set(mana); }
    double max_mana() { return mana_.max_value(); }

    int sight_count() { return sight_count_; }
    void set_sight_count(int sight_count) { sight_count_ += sight_count; }

    skills::usearguments::Aim& aim() { return aim_; }

    virtual bool AddCondition(sprite::Condition* new_condition);
    virtual void UpdateCondition(double dt);

    void set_weapon(skills::Skill *weapon) { active_skills_[Controller::PRIMARY] = weapon; }

  protected:
    friend class RectCollision;

    virtual void AddKnownCollisions();

    virtual void Update(double dt);
    virtual void PlayHitSound() const = 0;

    void Move(ugdk::Vector2D direction, double delta_t);
    void Move(ugdk::Vector2D distance);
    double GetAttackingAngle(ugdk::Vector2D targetDirection);
    int GetAttackingAnimationIndex(double angle);
    virtual ugdk::Vector2D GetWalkingDirection() {
        return walking_direction_;
    }
    void CollideWithRect(const pyramidworks::collision::CollisionObject*);


    /// The owner.
    sprite::WorldObject* owner_;
    
    ///
    Direction animation_direction_;

    /// 
    Direction last_standing_direction_;

    /// The active weapons this creature has.
    std::map<Controller::SkillSlot, skills::Skill*> active_skills_;

    /// The last position this creature was that is guaranteed to not colide with any walls.
    ugdk::Vector2D last_stable_position_;

    double last_dt_;

    /// The mana of this creature. An energy manages reneration.
    resource::Energy mana_;

    /// How many sight buffs this creature has.
    int sight_count_;

    /// How fast this creature moves per second.
    double speed_;

    /// Stores the original speed, so one can alter the speed temporarily.
    double original_speed_;

    /// The direction this creature is moving to.
    ugdk::Vector2D walking_direction_;

    // The conditions currently affecting this creature.
    std::list<sprite::Condition*> conditions_;

    /// Where this creature is aiming.
    skills::usearguments::Position aim_destination_;

    /// An aim resource. It's origin points to the creature's position and the destination to the creature's aim.
    skills::usearguments::Aim aim_;

};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_
