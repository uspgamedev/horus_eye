//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/door.h
// Definicao da classe Door
//

#ifndef HORUSEYE_GAME_SPRITE_DOOR_H_
#define HORUSEYE_GAME_SPRITE_DOOR_H_

#include "worldobject.h"
namespace framework{
	class Image;
}

namespace sprite {

class Hero;

class Door: public sprite::WorldObject {
  public:
    Door(framework::Image* image);
    virtual void HandleCollision(WorldObject *);
    void CollidesWith(Hero *);
    virtual ~Door() {}
};

}

#endif /* DOOR_H_ */
