#ifndef HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_
#define HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_

#include "imagescene.h"
#include <ugdk/math/vector2D.h>

namespace scene {

class ScrollingImageScene: public scene::ImageScene {
  public:
    ScrollingImageScene(ugdk::graphic::Drawable *background, ugdk::graphic::Drawable *image, double time);
    virtual void Update (double delta_t);

  private:
    double time_;
    ugdk::Vector2D movement_;
};

}

#endif /* HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_ */
