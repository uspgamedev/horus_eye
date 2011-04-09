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
    // TODO FIXME remake light system!!!
    Floor(framework::Image* image);
    virtual void Update(float delta_t);

    static void InitializeAnimations();
    static void ReleaseAnimations();

  private:
    utils::Tile *tile_;

    static framework::AnimationSet  *ANIMATIONS;
    static framework::uint32        CLEAR, DARK;

};

}

#endif /* FLOOR_H_ */
