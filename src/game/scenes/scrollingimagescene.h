#ifndef HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_
#define HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_

#include <ugdk/graphic.h>
#include "imagescene.h"

namespace scene {

class ScrollingImageScene: public scene::ImageScene {
  public:
    ScrollingImageScene(ugdk::graphic::Drawable *background, ugdk::graphic::Drawable *image, double time);
};

}

#endif /* HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_ */
