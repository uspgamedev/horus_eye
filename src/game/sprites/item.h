//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/lifepotion.h
// lifepotion.h da classe LifePotion
//

#ifndef HORUSEYE_GAME_SPRITES_ITEM_H_
#define HORUSEYE_GAME_SPRITES_ITEM_H_

#include "worldobject.h"
#include "itemevent.h"
#include "conditionbuilder.h"
#include "condition.h"

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

#endif /* HORUSEYE_GAME_SPRITES_ITEM_H_ */
