#ifndef HORUSEYE_GAME_SCENES_IMAGESCENE_H_
#define HORUSEYE_GAME_SCENES_IMAGESCENE_H_

#define StarWars ImageScene

#include <ugdk/action/scene.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>

namespace scene {

class ImageScene: public ugdk::Scene {
  typedef ugdk::Scene super;
  public:
    ImageScene(ugdk::Drawable *background, ugdk::Drawable *image);
    virtual ~ImageScene();

    virtual void Update(float delta_t);

  protected:
    void End();
    ugdk::Node *interface_node_, *scene_layers_[2];
};

}

#endif /* HORUSEYE_GAME_SCENES_IMAGESCENE_H_ */
