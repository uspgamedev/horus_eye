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

    virtual void Update(float delta_t);
    void StartAttack();
    pair<int, pbb> Think();
};

}
#endif // MUMMY_H_
