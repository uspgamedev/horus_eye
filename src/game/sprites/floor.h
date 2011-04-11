#ifndef FLOOR_H_
#define FLOOR_H_

#include "worldobject.h"
namespace framework {
	class Image;
    class AnimationSet;
};
namespace utils {
    class Tile;
};
namespace sprite {

class Floor : public WorldObject {

  public:
    Floor(framework::Image* image);
    virtual void Update(float delta_t);

  private:
    utils::Tile *tile_;

};

}

#endif /* FLOOR_H_ */
