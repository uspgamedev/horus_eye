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
#include "worldobject.h"
#include "wall.h"

namespace sprite {

// Classe Creature

class Creature : public sprite::WorldObject {
  public:
    Creature();
    virtual ~Creature() { }

    // Colisoes
    void CollidesWith(Wall *);

  protected:

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
