#ifndef HORUSEYE_FRONTEND_SCENES_IMAGESCENE_H_
#define HORUSEYE_FRONTEND_SCENES_IMAGESCENE_H_

#include <ugdk/action/scene.h>
#include <ugdk/ui.h>

#include <memory>

namespace frontend {
namespace scenes {

class ImageScene: public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    ImageScene(std::unique_ptr<ugdk::ui::Drawable>&& background, std::unique_ptr<ugdk::ui::Drawable>&& image);
    virtual ~ImageScene();

  protected:
    void End();
    std::unique_ptr<ugdk::ui::Node> scene_layers_[2];
};

} // namespace scenes
} // namespace frontend

#endif // HORUSEYE_FRONTEND_SCENES_IMAGESCENE_H_
