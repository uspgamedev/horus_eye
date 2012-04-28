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

class Creature : public Logic, public ugdk::action::Observer { 
  public:
    Creature(sprite::WorldObject* owner, Controller* controller);
    virtual ~Creature();

    sprite::WorldObject* owner() { return owner_; }

    resource::Energy& mana() { return mana_; }
    void set_mana(resource::Energy &mana) {
        mana_ = mana;
    }
    void set_mana(double mana) {
        mana_.Set(mana);
    }
    double max_mana() { return mana_.max_value(); }

    int sight_count() { return sight_count_; }
    void set_sight_count(int sight_count) { sight_count_ += sight_count; }

    skills::usearguments::Aim& aim() { return aim_; }

    virtual bool AddCondition(sprite::Condition* new_condition);
    virtual void UpdateCondition(double dt);
    void set_weapon(skills::Skill *weapon) { active_skills_[Controller::PRIMARY] = weapon; }

    // Colisoes
    static void InitializeAnimations();
    static void ReleaseAnimations() { ANIMATIONS = NULL; }

  protected:
    static const Direction direction_mapping_[8];
    static ugdk::uint32 standing_animations_[16];
    static ugdk::uint32 walking_animations_[16];
    static ugdk::uint32 attacking_animations_[8];
    static ugdk::uint32 taking_damage_animation_;
    static ugdk::uint32 dying_animation_;
    static ugdk::action::AnimationSet *ANIMATIONS;
    static ugdk::Vector2D directions_[4];

    friend class RectCollision;
    
    class Direction_ {
      public:
        static const int RIGHT = 0;
        static const int LEFT = 1;
        static const int UP = 2;
        static const int DOWN = 3;
    };

    class Animation_ {
      public:
        static const int RIGHT = 1;
        static const int LEFT = 2;
        static const int UP = 4;
        static const int DOWN = 8;
    };

    void Initialize(ugdk::graphic::Spritesheet *image, ugdk::action::AnimationSet *set = NULL);
    virtual void AddKnownCollisions();

    virtual void Dying(double dt) {}
    virtual void Update(double dt);
    virtual void PlayHitSound() const {}

    // funcoes
    void Move(ugdk::Vector2D direction, double delta_t);
    void Move(ugdk::Vector2D distance);
    void Tick();
    double GetAttackingAngle(ugdk::Vector2D targetDirection);
    int GetAttackingAnimationIndex(double angle);
    virtual ugdk::Vector2D GetWalkingDirection() {
        return walking_direction_;
    }
    void CollideWithRect(const pyramidworks::collision::CollisionObject*);
    static void InitializeStandingAnimations();
    static void InitializeWalkingAnimations();
    static void InitializeAttackingAnimations();


    /// The owner.
    sprite::WorldObject* owner_;

    /// Is this creature waiting for an animation to finish?
    bool waiting_animation_;
    
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

    /// Well, kinda hacky or not. TODO better comment
    ugdk::graphic::Sprite* sprite_;

};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_
