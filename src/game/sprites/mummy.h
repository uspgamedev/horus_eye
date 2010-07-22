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
#include "../../framework/timeaccumulator.h"
//#include <algorithm>
#include<queue>
#define pbb pair<bool, bool>

using namespace std;
using namespace framework;

namespace sprite {

class Mummy : public Creature, public Observer {

  public:
    Mummy(Image* img = NULL);
    ~Mummy();
    virtual void HandleCollision(WorldObject *);
    virtual void CollidesWith(Projectile *);
    virtual void CollidesWith(Hero *);
    virtual void CollidesWith(Mummy *);
  private:

    TimeAccumulator *interval_;
    Vector2D         last_direction_;
    queue<Vector2D> path_;

    virtual void Update(float delta_t);
    void StartAttack();
    void Think();
    void RandomMovement();
    void Tick();
    void PlayHitSound() const;
};

}
#endif // MUMMY_H_
