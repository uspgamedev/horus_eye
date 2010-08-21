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

class Potion: public sprite::WorldObject {
public:
    Potion (framework::Image* img = NULL);
    ~Potion ();
    
    virtual void Update(float delta_t);

    virtual void CollidesWith(Hero *obj);
    virtual void HandleCollision(WorldObject *);

    int recover_life() { return recover_life_; }
    int recover_mana() { return recover_mana_; }
    void set_recover_life(int life) { recover_life_ = life; }
    void set_recover_mana(int mana) { recover_mana_ = mana; }
     
private:
    int recover_life_, recover_mana_;
    float total_time_;
};

}

#endif /* LIFEPOTION_H_ */
