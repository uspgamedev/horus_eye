#ifndef HORUSEYE_GAME_SCENES_IMAGESCENE_H_
#define HORUSEYE_GAME_SCENES_IMAGESCENE_H_

#define StarWars ImageScene

#include "../../framework/scene.h"
#include "../../framework/image.h"

namespace scene {

class ImageScene: public ugdk::Scene {
  public:
    ImageScene(ugdk::Image *background, ugdk::Image *image);
    virtual ~ImageScene ();
    virtual void Update (float delta_t);

  protected:
    void End ();
    ugdk::Layer *scene_layers_[2];
};

}

#endif /* HORUSEYE_GAME_SCENES_IMAGESCENE_H_ */
