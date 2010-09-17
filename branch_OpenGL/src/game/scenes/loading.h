#ifndef HORUSEYE_GAME_SCENES_LOADING_H_
#define HORUSEYE_GAME_SCENES_LOADING_H_

#include "../../framework/scene.h"

namespace framework {
class Layer;
class Image;
class Sprite;
}

namespace scene {

class Loading: public framework::Scene {
  public:
    Loading();
    virtual ~Loading();
    virtual void Update(float delta_t);

  protected:
    framework::Image* loading_image_;
    bool has_been_drawn_;
};

}

#endif /* HORUSEYE_GAME_SCENES_LOADING_H_ */
