#ifndef HORUSEYE_GAME_SPRITE_DOOR_H_
#define HORUSEYE_GAME_SPRITE_DOOR_H_

#include "worldobject.h"

namespace ugdk {
class Image;
}

namespace sprite {

class Door: public sprite::WorldObject {
  DEFINE_COLLIDABLE
  public:
    Door(ugdk::Image* image);
    virtual ~Door() {}

    struct Collisions {
        class Win : public CollisionObject {
          public:
            Win() {}
            void Handle(WorldObject* obj);
        };
    };
};

}

#endif /* HORUSEYE_GAME_SPRITE_DOOR_H_ */
