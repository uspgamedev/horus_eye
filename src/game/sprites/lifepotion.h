//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/lifepotion.h
// lifepotion.h da classe LifePotion
//

#ifndef LIFEPOTION_H_
#define LIFEPOTION_H_

#include "worldobject.h"
#include "../../framework/sprite.h"
#include "../../framework/engine.h"

namespace sprite {

class LifePotion: public sprite::WorldObject {
public:
    LifePotion ();
    ~LifePotion ();
    
    virtual void CollidesWith(Hero *obj);
    virtual void HandleCollision(WorldObject *);
    int recoveramount() { return recover_life_; }
     
private:
    int recover_life_;
};

}

#endif /* LIFEPOTION_H_ */
