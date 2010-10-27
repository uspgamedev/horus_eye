#ifndef HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_
#define HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_

#include "imagescene.h"
#include "../../framework/vector2D.h"

namespace scene {

class ScrollingImageScene: public scene::ImageScene {
  public:
    ScrollingImageScene(framework::Image *background,
            framework::Image *image, float time);
    virtual void Update (float delta_t);

  private:
    float time_;
    framework::Vector2D movement_;
};

}

#endif /* HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_ */
