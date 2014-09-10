#ifndef HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_
#define HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_

#include "imagescene.h"

#include <ugdk/ui.h>

namespace scene {

class ScrollingImageScene: public scene::ImageScene {
  public:
    ScrollingImageScene(std::unique_ptr<ugdk::ui::Drawable>&& background, std::unique_ptr<ugdk::ui::Drawable>&& image, double time);
};

}

#endif /* HORUSEYE_GAME_SCENES_SCROLLINGIMAGESCENE_H_ */
