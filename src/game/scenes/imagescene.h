//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/imagescene.h
// Definicao da classe ImageScene.
//

#ifndef HORUSEYE_GAME_SCENES_TEXTSCENE_H_
#define HORUSEYE_GAME_SCENES_TEXTSCENE_H_

#define StarWars ImageScene

#include "../../framework/vector2D.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/timeaccumulator.h"

namespace scene {

class ImageScene: public framework::Scene {
  public:
    typedef enum {
        INTRO,
        ENDING,
        GAMEOVER
    } SceneType;

    ImageScene (framework::Image *background, framework::Image *image,
                float time, SceneType type);
    virtual ~ImageScene ();

    void Update (float delta_t);

  protected:

    void End ();

  private:
    float time_;
    framework::Layer *scene_layers_[2];
    framework::Vector2D movement_;
    SceneType type_;
};

}

#endif /* TEXTSCENE_H_ */
