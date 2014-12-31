#ifndef HORUSEYE_FRONTEND_SCENES_SCROLLINGIMAGESCENE_H_
#define HORUSEYE_FRONTEND_SCENES_SCROLLINGIMAGESCENE_H_

#include "imagescene.h"

#include <ugdk/ui.h>

namespace frontend {
namespace scenes {

class ScrollingImageScene: public ImageScene {
  public:
    ScrollingImageScene(std::unique_ptr<ugdk::ui::Drawable>&& background, std::unique_ptr<ugdk::ui::Drawable>&& image, double time);
};

} // namespace scenes
} // namespace frontend

#endif // HORUSEYE_FRONTEND_SCENES_SCROLLINGIMAGESCENE_H_ 
