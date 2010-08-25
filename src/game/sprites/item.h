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
#include "itemevent.h"

namespace sprite {

class Item: public sprite::WorldObject {
  public:
    Item (framework::Image* img = NULL);
    ~Item () { if (event_) delete event_; }
    
    virtual void Update(float delta_t);
    virtual void set_event(ItemEvent *event) { event_ = event; }

    virtual void CollidesWith(Hero *obj);
    virtual void HandleCollision(WorldObject *);
     
  private:
    ItemEvent *event_;
    float total_time_;
};

}

#endif /* LIFEPOTION_H_ */
