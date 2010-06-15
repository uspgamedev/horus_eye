//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.h
// Definicao da classe Hero.
//

#ifndef HORUSEYE_GAME_SPRITE_HERO_H_
#define HORUSEYE_GAME_SPRITE_HERO_H_


#include "creature.h"

using namespace std;
using namespace framework;

namespace sprite {


class Hero : public Creature {

  public:
    Hero();
    ~Hero() {} 
  private:
    int animation_direction_;
    Animation *last_standing_animation_;
    Animation ** standing_animations_[16];
    Animation ** walking_animations_[16];

    Vector2D directions_[4];
    bool pressed_key_[4];

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


    void SelectSpriteAnimation(); 
    virtual void Update(float delta_t);
    void Move(float delta_t);
    void GetKeys();
};

}
#endif // HERO_H_
