//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/creature.h
// Definicao da classe World.
//

#ifndef HORUSEYE_GAME_SPRITE_CREATURE_H_
#define HORUSEYE_GAME_SPRITE_CREATURE_H_

#include "../../framework/sprite.h"
#include "../../framework/vector2D.h"
#include "../../framework/timeaccumulator.h"
#include "../utils/rectobject.h"
#include "door.h"
#include "wall.h"
#include "worldobject.h"
namespace sprite {

// Classe Creature

using namespace framework;
using namespace utils;

class Creature : public WorldObject , public Observer {
  public:
    Creature();
    virtual ~Creature();

    bool is_attacking() { return is_attacking_; }
    int life() { return life_; }
    int max_life() { return  max_life_; }
    int mana() { return mana_; }
    int max_mana() { return  max_mana_; }

    // Colisoes
    virtual void CollidesWith(Wall *);
    virtual void CollidesWith(Door *);

    virtual void HandleCollision(WorldObject *);

  protected:
    int animation_direction_;
    int direction_mapping_[8];
    bool is_attacking_;
    bool is_taking_damage_;
    Animation *last_standing_animation_;
    Animation ** standing_animations_[16];
    Animation ** walking_animations_[16];
    Animation * attacking_animations_[8];
    Animation * taking_damage_animation_;
    Animation * dying_animation_;

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

    virtual void Update(float dt) { WorldObject::Update(dt); }

    // funcoes
    void Move(framework::Vector2D direction, float delta_t);
    void Tick();
    void SelectSpriteAnimation(Animation *animation);
    double GetAttackingAngle(Vector2D targetDirection);
    int GetAttackingAnimationIndex(double angle);
    void InitializeStandingAnimations();
    void InitializeWalkingAnimations();
    void InitializeAttackingAnimations();
    virtual framework::Vector2D GetWalkingDirection() {
        return walking_direction_;
    }
    void CollideWithRect(const RectObject*);

    // variaveis
    Vector2D last_stable_position_;
    int   life_, max_life_, mana_, max_mana_;
    float speed_, attack_cool_down_, attack_duration_;
    framework::TimeAccumulator *hit_duration_;
    framework::Vector2D walking_direction_, looking_direction_;



};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_
