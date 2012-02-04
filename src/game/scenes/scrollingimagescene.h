#ifndef HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_
#define HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_

#include "imagescene.h"
#include <ugdk/math/vector2D.h>

namespace scene {

class ScrollingImageScene: public scene::ImageScene {
  public:
    ScrollingImageScene(ugdk::graphic::Drawable *background, ugdk::graphic::Drawable *image, float time);
    virtual void Update (float delta_t);

  private:
    float time_;
    ugdk::Vector2D movement_;
};

}

#endif /* HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_ */
