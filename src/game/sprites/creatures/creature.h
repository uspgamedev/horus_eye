#ifndef HORUSEYE_GAME_SPRITE_CREATURE_H_
#define HORUSEYE_GAME_SPRITE_CREATURE_H_

#include <list>

#include <ugdk/action/sprite.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/time/timeaccumulator.h>
#include <ugdk/action/observer.h>
#include <pyramidworks/geometry/rect.h>
#include "game/sprites/condition.h"
#include "game/sprites/worldobject.h"
#include <game/resources/simpleresource.h>
#include "game/skills/castarguments.h"

namespace ugdk {
class TimeAccumulator;
class Animation;
class AnimationSet;
}

namespace skills {
class Skill;
}

namespace sprite {

using ugdk::Vector2D;

class Door;
class Wall;
class Block;

class Creature : public WorldObject , public ugdk::Observer {
  
  public:
    Creature();
    virtual ~Creature();

    void Initialize(ugdk::Drawable *image,
                    ugdk::AnimationSet *set = NULL,
                    bool delete_image = false);

    resource::SimpleResource& life() { return life_; }
	void set_life(resource::SimpleResource &life) {
		life_ = life;
	}
	void set_life(float life) {
	    life_.Set(life);
	}

    float mana() { return mana_; }
    void set_mana(resource::SimpleResource &mana) {
        mana_ = mana;;
    }
	void set_mana(float mana) {
		mana_.Set(mana);
	}
    float max_mana() { return mana_.max_value(); }

    int sight_count() { return sight_count_; }
    void set_sight_count(int sight_count) { sight_count_ += sight_count; }

    void set_super_armor(bool super_armor) { super_armor_ = super_armor; }

    skills::castarguments::Aim* aim() { return aim_; }

    virtual bool AddCondition(Condition* new_condition);
    virtual void UpdateCondition(float dt);
    virtual void TakeDamage(float life_points);
    void set_weapon(skills::Skill *weapon) { weapon_ = weapon; }

    // Colisoes
    static void InitializeAnimations();
    static void ReleaseAnimations() { ANIMATIONS = NULL; }

  protected:
	bool waiting_animation_;
    int animation_direction_;
    ugdk::uint32 last_standing_animation_;
    
    static int direction_mapping_[8];
    static ugdk::uint32 standing_animations_[16];
    static ugdk::uint32 walking_animations_[16];
    static ugdk::uint32 attacking_animations_[8];
    static ugdk::uint32 taking_damage_animation_;
    static ugdk::uint32 dying_animation_;
    static ugdk::AnimationSet *ANIMATIONS;
    static ugdk::Vector2D directions_[4];


	COLLISION_BEGIN
		COLLISION_ADD		(Creature, Rect)
	COLLISION_END
    
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

    virtual void Update(float dt) { UpdateCondition(dt); WorldObject::Update(dt); }
	virtual void Render();
    virtual void PlayHitSound() const {}

    // funcoes
    void AdjustBlink(float delta_t);
    void Move(ugdk::Vector2D direction, float delta_t);
    void Tick();
    float GetAttackingAngle(Vector2D targetDirection);
    int GetAttackingAnimationIndex(float angle);
    virtual ugdk::Vector2D GetWalkingDirection() {
        return walking_direction_;
    }
    void CollideWithRect(const pyramidworks::geometry::Rect*);
    static void InitializeStandingAnimations();
    static void InitializeWalkingAnimations();
    static void InitializeAttackingAnimations();

    // The base weapon this creature uses.
    skills::Skill *weapon_;

    // The last position this creature was that is guaranteed to not colide with any walls.
    Vector2D last_stable_position_;

    resource::SimpleResource life_, mana_;
    float /*life_, max_life_, mana_, max_mana_, */ mana_regen_;

    // How many sight buffs this creature has.
    int sight_count_;

    // When true, this creature does not flinch when hit.
    bool super_armor_;

    // For how much time this creature will be invulnerable after taking a hit.
    int invulnerability_time_;

    // When true, this Creature is on the invisible part of the blinking effect.
    bool blink_;

    // Controls when to toggle the blink_ flag.
    ugdk::TimeAccumulator *blink_time_;

    // Controls the invulnerability after being hit.
    ugdk::TimeAccumulator *hit_duration_;

    // How fast this creature moves per second.
    float speed_;

    // Stores the original speed, so one can alter the speed temporarily.
    float original_speed_;

    ugdk::Vector2D walking_direction_;

    // The conditions currently affecting this creature.
    std::list<Condition*> conditions_;

    // Where this creature is aiming.
    skills::castarguments::Aim* aim_;

};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_