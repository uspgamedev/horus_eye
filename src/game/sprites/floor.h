#ifndef FLOOR_H_
#define FLOOR_H_

#include "worldobject.h"
namespace framework {
	class Image;
	class Animation;
};
namespace utils {
    class Tile;
};
namespace sprite {

class Floor : public WorldObject {

  public:
    Floor(framework::Image* image);
    virtual void Update(float delta_t);
    virtual void Render(framework::Image *back_buffer, framework::Vector2D &offset);

  private:
    utils::Tile *tile_;
    framework::Animation *clear_, *dark_;

};

}

#endif /* FLOOR_H_ */
