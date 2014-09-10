#ifndef HORUSEYE_GAME_SCENES_IMAGESCENE_H_
#define HORUSEYE_GAME_SCENES_IMAGESCENE_H_

#define StarWars ImageScene

#include <ugdk/action/scene.h>
#include <ugdk/ui.h>

#include <memory>

namespace scene {

class ImageScene: public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    ImageScene(std::unique_ptr<ugdk::ui::Drawable>&& background, std::unique_ptr<ugdk::ui::Drawable>&& image);
    virtual ~ImageScene();

  protected:
    void End();
    std::unique_ptr<ugdk::ui::Node> scene_layers_[2];
};

}

#endif /* HORUSEYE_GAME_SCENES_IMAGESCENE_H_ */
