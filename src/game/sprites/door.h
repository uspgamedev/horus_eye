#ifndef HORUSEYE_GAME_SPRITE_DOOR_H_
#define HORUSEYE_GAME_SPRITE_DOOR_H_

#include "worldobject.h"
namespace ugdk{
	class Image;
}

namespace sprite {

class Hero;

class Door: public sprite::WorldObject {
  public:
    Door(ugdk::Image* image);
    virtual ~Door() {}
    
    void CollidesWith(Hero *);

    
    
};

}

#endif /* DOOR_H_ */
