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
#include "wall.h"
#include "worldobject.h"
namespace sprite {

// Classe Creature

class Creature : public WorldObject {
  public:
    Creature();
    virtual ~Creature() { }

    // Colisoes
    virtual void CollidesWith(Wall *);

  protected:

    virtual void Update(float dt) { WorldObject::Update(dt); }

    // funcoes
    void Move(framework::Vector2D direction, float delta_t);
    virtual framework::Vector2D GetWalkingDirection() {
        return walking_direction_;
    }

    // variaveis
    int   life_, max_life_;
    float speed_, attack_cool_down_, attack_duration_, hit_duration_;
    framework::Vector2D walking_direction_, looking_direction_;
};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_CREATURE_H_
