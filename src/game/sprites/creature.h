#ifndef HORUSEYE_GAME_SPRITE_CREATURE_H_
#define HORUSEYE_GAME_SPRITE_CREATURE_H_

#include <list>
#include "../../framework/sprite.h"
#include "../../framework/vector2D.h"
#include "../../framework/timeaccumulator.h"
#include "../../framework/observer.h"
#include "../utils/rectobject.h"
#include "condition.h"
#include "worldobject.h"

namespace ugdk {
class TimeAccumulator;
class Animation;
class AnimationSet;
}

namespace sprite {

using ugdk::Vector2D;

class Weapon;
class Door;
class Wall;

class Creature : public WorldObject , public ugdk::Observer {
  public:
    Creature();
    virtual ~Creature();

    void Initialize(ugdk::Drawable *image,
                    ugdk::AnimationSet *set = NULL,
                    bool delete_image = false);

    float life() { return life_; }
	void set_life(float life) {
		life_ = life;
		if (life_ < 0.0f) life_ = 0.0f;
		if (life_ > max_life_) life_ = max_life_;
	}
    float max_life() { return  max_life_; }

    float mana() { return mana_; }
	void set_mana(float mana) {
		mana_ = mana;
		if (mana_ < 0.0f) mana_ = 0.0f;
		if (mana_ > max_mana_) mana_ = max_mana_;
	}
    float max_mana() { return  max_mana_; }

    int sight_count() { return sight_count_; }
    void set_sight_count(int sight_count) { sight_count_ += sight_count; }

    void set_super_armor(bool super_armor) { super_armor_ = super_armor; }

    virtual bool AddCondition(Condition* new_condition);
    virtual void UpdateCondition(float dt);
    virtual void TakeDamage(float life_points);
    void set_weapon(Weapon *weapon) { weapon_ = weapon; }

    // Colisoes
    virtual void CollidesWith(Wall *);
    virtual void CollidesWith(Door *);

    virtual void HandleCollision(WorldObject *);

    static void InitializeAnimations();
    static void ReleaseAnimations();

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
    void CollideWithRect(const utils::RectObject*);
    static void InitializeStandingAnimations();
    static void InitializeWalkingAnimations();
    static void InitializeAttackingAnimations();

    // variaveis
    Weapon *weapon_;
    Vector2D last_stable_position_;
    float life_, max_life_, mana_, max_mana_, mana_regen_;
    int sight_count_;

    // When true, this creature does not flinch when hit.
    bool super_armor_;

    // For how much time this creature will be invulnerable after taking a hit.
    int invulnerability_time_;

    // When true, this Creature is on the invisible part of the blinking effect.
    bool blink_;

    // Controls when to toggle the blink_ flag.
    ugdk::TimeAccumulator *blink_time_;

    float original_speed_, speed_, attack_cool_down_, attack_duration_;
    ugdk::TimeAccumulator *hit_duration_;
    ugdk::Vector2D walking_direction_, looking_direction_;
    std::list<Condition*> conditions_;

};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_
