//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.h
// Definicao da classe Mummy.
//

#ifndef HORUSEYE_GAME_SPRITE_MUMMY_H_
#define HORUSEYE_GAME_SPRITE_MUMMY_H_


#include "creature.h"
#include "../../framework/animation.h"
#include "../../framework/vector2D.h"
#include <algorithm>
#define pbb pair<bool, bool>

using namespace std;
using namespace framework;




namespace sprite {


class Mummy : public Creature {

  public:
    Mummy();
    ~Mummy() {} 
  private:
    int animation_direction_;
    int direction_mapping_[8];
    bool is_attacking_;
    Animation *last_standing_animation_;
    Animation ** standing_animations_[16];
    Animation ** walking_animations_[16];
    Animation * attacking_animations_[8];



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

    void SelectSpriteAnimation(Animation *, Vector2D);
    double GetAttackingAngle(Vector2D mousePosition);
    int GetAttackingAnimationIndex(double angle);
    virtual void Update(float delta_t);
    void StartAttack();
    void GetMouseState();
    pair<int, pbb> Think();
};

}
#endif // MUMMY_H_
