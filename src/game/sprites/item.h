#ifndef HORUSEYE_GAME_SPRITES_ITEM_H_
#define HORUSEYE_GAME_SPRITES_ITEM_H_

#include "worldobject.h"
#include "itemevent.h"
#include "conditionbuilder.h"
#include "condition.h"

namespace sprite {

class Item: public sprite::WorldObject {
  public:
    Item (ugdk::Image* img);
    ~Item () { if (event_) delete event_; }
    
    virtual void Update(float delta_t);
    virtual void set_event(ItemEvent *event) { event_ = event; }

  private:
    ItemEvent *event_;
    float total_time_;

    COLLISION_BEGIN
		COLLISION_ADD (Item, Use)
	COLLISION_END
};

}

#endif /* HORUSEYE_GAME_SPRITES_ITEM_H_ */
