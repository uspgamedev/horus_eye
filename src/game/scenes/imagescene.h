#ifndef HORUSEYE_GAME_SCENES_IMAGESCENE_H_
#define HORUSEYE_GAME_SCENES_IMAGESCENE_H_

#define StarWars ImageScene

#include <ugdk/action/scene.h>
#include <ugdk/graphic.h>

namespace scene {

class ImageScene: public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    ImageScene(ugdk::graphic::Drawable *background, ugdk::graphic::Drawable *image);
    virtual ~ImageScene();

  protected:
    void End();
    ugdk::graphic::Node *scene_layers_[2];
};

}

#endif /* HORUSEYE_GAME_SCENES_IMAGESCENE_H_ */
