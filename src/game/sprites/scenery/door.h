#ifndef HORUSEYE_GAME_SPRITE_DOOR_H_
#define HORUSEYE_GAME_SPRITE_DOOR_H_

#include "game/sprites/worldobject.h"

namespace ugdk {
class Image;
}

namespace sprite {

class Door: public sprite::WorldObject {
  
  public:
    Door(ugdk::Image* image);
    virtual ~Door() {}

	COLLISION_BEGIN
		COLLISION_ADD		(Door, Win)
	COLLISION_END
};

}

#endif /* HORUSEYE_GAME_SPRITE_DOOR_H_ */
