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
#include "wall.h"
#include "worldobject.h"
namespace sprite {

// Classe Creature

using namespace framework;

class Creature : public WorldObject {
  public:
    Creature();
    virtual ~Creature() { }

    bool is_attacking() { return is_attacking_; }

    // Colisoes
    virtual void CollidesWith(Wall *);

    virtual void HandleCollision(WorldObject *);

  protected:
    int animation_direction_;
    int direction_mapping_[8];
    bool is_attacking_;
    Animation *last_standing_animation_;
    Animation ** standing_animations_[16];
    Animation ** walking_animations_[16];
    Animation * attacking_animations_[8];
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
    void SelectSpriteAnimation(Animation *animation, Vector2D frame_size);
    virtual framework::Vector2D GetWalkingDirection() {
        return walking_direction_;
    }

    // variaveis
    Vector2D last_stable_position_;
    int   life_, max_life_;
    float speed_, attack_cool_down_, attack_duration_;
    framework::TimeAccumulator *hit_duration_;
    framework::Vector2D walking_direction_, looking_direction_;
};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_
