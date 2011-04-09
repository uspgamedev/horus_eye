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

namespace framework {
class TimeAccumulator;
class Animation;
class AnimationSet;
}

namespace sprite {

using framework::Vector2D;

class Weapon;
class Door;
class Wall;

class Creature : public WorldObject , public framework::Observer {
  public:
    Creature();
    virtual ~Creature();

    void Initialize(framework::Drawable *image,
                    framework::AnimationSet *set = NULL,
                    bool delete_image = false);

    int life() { return life_; }
	void set_life(int life) {
		life_ = life;
		if (life_ < 0) life_ = 0;
		if (life_ > max_life_) life_ = max_life_;
	}
    int max_life() { return  max_life_; }
    int mana() { return mana_; }
	void set_mana(int mana) {
		mana_ = mana;
		if (mana_ < 0) mana_ = 0;
		if (mana_ > max_mana_) mana_ = max_mana_;
	}
    int max_mana() { return  max_mana_; }
    int sight_count() { return sight_count_; }
    void set_sight_count(int sight_count) { sight_count_ += sight_count; }
    virtual bool AddCondition(Condition* new_condition);
    virtual void UpdateCondition(float dt);
    virtual void TakeDamage(int life_points = 1);
    void set_weapon(Weapon *weapon) { weapon_ = weapon; }

    // Colisoes
    virtual void CollidesWith(Wall *);
    virtual void CollidesWith(Door *);

    virtual void HandleCollision(WorldObject *);

    static void InitializeAnimations();
    static void ReleaseAnimations();


    // TODO: fixme
    static framework::AnimationSet *GET_ANIMATIONS() { return ANIMATIONS; }

  protected:
	bool waiting_animation_;
    int animation_direction_;
    int direction_mapping_[8];
    /*
    Animation *last_standing_animation_;
    Animation ** standing_animations_[16];
    Animation ** walking_animations_[16];
    Animation * attacking_animations_[8];
    Animation * taking_damage_animation_;
    Animation * dying_animation_;
    */
    Weapon *weapon_;

    int last_standing_animation_;

    static int standing_animations_[16];
    static int walking_animations_[16];
    static int attacking_animations_[8];
    static int taking_damage_animation_;
    static int dying_animation_;

    static framework::AnimationSet *ANIMATIONS;

    Vector2D directions_[4];
    
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

    // funcoes
    void AdjustBlink(float delta_t);
    void Move(framework::Vector2D direction, float delta_t);
    void Tick();
    float GetAttackingAngle(Vector2D targetDirection);
    int GetAttackingAnimationIndex(float angle);
    virtual framework::Vector2D GetWalkingDirection() {
        return walking_direction_;
    }
    void CollideWithRect(const utils::RectObject*);
    static void InitializeStandingAnimations();
    static void InitializeWalkingAnimations();
    static void InitializeAttackingAnimations();

    // variaveis
    Vector2D last_stable_position_;
    int   life_, max_life_, mana_, max_mana_, sight_count_;
	double blink_time_;
    bool blink_;
    float original_speed_, speed_, attack_cool_down_, attack_duration_;
    framework::TimeAccumulator *hit_duration_;
    framework::Vector2D walking_direction_, looking_direction_;
    std::list<Condition*> conditions_;

};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_
