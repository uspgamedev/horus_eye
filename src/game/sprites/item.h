#ifndef HORUSEYE_GAME_SPRITES_ITEM_H_
#define HORUSEYE_GAME_SPRITES_ITEM_H_

#include "game/sprites/worldobject.h"
#include "game/sprites/itemevent.h"
#include "game/builders/conditionbuilder.h"
#include "game/sprites/condition.h"

namespace sprite {

class Item: public sprite::WorldObject {
  public:
    Item (ugdk::graphic::FlexibleSpritesheet* img);
    ~Item () { if (event_) delete event_; }
    
    virtual void Update(float delta_t);
    virtual void set_event(ItemEvent *event) { event_ = event; }

  private:
    ItemEvent *event_;
    float total_time_;

    friend class UseCollision;
};

}

#endif /* HORUSEYE_GAME_SPRITES_ITEM_H_ */
