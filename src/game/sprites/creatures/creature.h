#ifndef HORUSEYE_GAME_SPRITE_CREATURE_H_
#define HORUSEYE_GAME_SPRITE_CREATURE_H_

#include <list>

#include <ugdk/action/animation.h>
#include <ugdk/action/observer.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/time.h>
#include "game/sprites/condition.h"
#include "game/sprites/worldobject.h"
#include <game/resources/energy.h>
#include "game/skills/usearguments.h"

namespace skills {
class Skill;
}

namespace pyramidworks {
namespace geometry {
        class Rect;
}
}

namespace sprite {

using ugdk::Vector2D;

class Creature : public WorldObject , public ugdk::Observer {
  
  public:
    Creature();
    virtual ~Creature();

    resource::Energy& life() { return life_; }
	void set_life(resource::Energy &life) {
		life_ = life;
	}
	void set_life(double life) {
	    life_.Set(life);
	}

	resource::Energy& mana() { return mana_; }
    void set_mana(resource::Energy &mana) {
        mana_ = mana;;
    }
	void set_mana(double mana) {
		mana_.Set(mana);
	}
    double max_mana() { return mana_.max_value(); }

    int sight_count() { return sight_count_; }
    void set_sight_count(int sight_count) { sight_count_ += sight_count; }

    void set_super_armor(bool super_armor) { super_armor_ = super_armor; }

    skills::usearguments::Aim& aim() { return aim_; }

    virtual bool AddCondition(Condition* new_condition);
    virtual void UpdateCondition(double dt);
    virtual void TakeDamage(double life_points);
    void set_weapon(skills::Skill *weapon) { weapon_ = weapon; }

    // Colisoes
    static void InitializeAnimations();
    static void ReleaseAnimations() { ANIMATIONS = NULL; }

  protected:  
    static int direction_mapping_[8];
    static ugdk::uint32 standing_animations_[16];
    static ugdk::uint32 walking_animations_[16];
    static ugdk::uint32 attacking_animations_[8];
    static ugdk::uint32 taking_damage_animation_;
    static ugdk::uint32 dying_animation_;
    static ugdk::AnimationSet *ANIMATIONS;
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

    Creature(resource::Energy &life, resource::Energy &mana);
    void Initialize(ugdk::graphic::Spritesheet *image, ugdk::AnimationSet *set = NULL);

    virtual void Update(double dt);
    virtual void PlayHitSound() const {}

    // funcoes
    void AdjustBlink(double delta_t);
    void Move(ugdk::Vector2D direction, double delta_t);
    void Move(ugdk::Vector2D distance);
    void Tick();
    double GetAttackingAngle(Vector2D targetDirection);
    int GetAttackingAnimationIndex(double angle);
    virtual ugdk::Vector2D GetWalkingDirection() {
        return walking_direction_;
    }
    void CollideWithRect(const pyramidworks::geometry::Rect*);
    static void InitializeStandingAnimations();
    static void InitializeWalkingAnimations();
    static void InitializeAttackingAnimations();

    /// Is this creature waiting for an animation to finish?
    bool waiting_animation_;
    
    ///
    int animation_direction_;

    /// 
    ugdk::uint32 last_standing_animation_;

    /// The base weapon this creature uses.
    skills::Skill* weapon_;

    /// The last position this creature was that is guaranteed to not colide with any walls.
    Vector2D last_stable_position_;

    double last_dt_;

    /// The life and mana of this creature. An energy manages reneration.
    resource::Energy life_, mana_;

    /// How many sight buffs this creature has.
    int sight_count_;

    /// When true, this creature does not flinch when hit.
    bool super_armor_;

    /// For how much time this creature will be invulnerable after taking a hit.
    int invulnerability_time_;

    /// Controls when to toggle the blink_ flag.
    ugdk::time::TimeAccumulator *blink_time_;

    /// Controls the invulnerability after being hit.
    ugdk::time::TimeAccumulator *hit_duration_;

    /// How fast this creature moves per second.
    double speed_;

    /// Stores the original speed, so one can alter the speed temporarily.
    double original_speed_;

    /// The direction this creature is moving to.
    ugdk::Vector2D walking_direction_;

    // The conditions currently affecting this creature.
    std::list<Condition*> conditions_;

    /// Where this creature is aiming.
    skills::usearguments::Position aim_destination_;

    /// An aim resource. It's origin points to the creature's position and the destination to the creature's aim.
    skills::usearguments::Aim aim_;

    /// Well, kinda hacky or not. TODO better comment
    ugdk::graphic::Sprite* sprite_;

  private:
    /// When true, this Creature is on the invisible part of the blinking effect.
    bool blink_;

};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_
