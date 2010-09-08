//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/floor.h
// Definicao da classe Floor
//

#ifndef FLOOR_H_
#define FLOOR_H_

#include "worldobject.h"
namespace framework{
	class Image;
};
namespace sprite {

class Floor : public WorldObject {

  public:
    Floor(framework::Image* image);
    virtual void Update(float delta_t);
};

}

#endif /* FLOOR_H_ */
