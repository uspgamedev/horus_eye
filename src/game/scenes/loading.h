#ifndef HORUSEYE_GAME_SCENES_LOADING_H_
#define HORUSEYE_GAME_SCENES_LOADING_H_

#include <ugdk/action/scene.h>

namespace scene {

class Loading: public ugdk::Scene {
  public:
    Loading();
    virtual ~Loading();
    virtual void Update(float delta_t);

  protected:
    bool has_been_drawn_;
    ugdk::Node *loading_;
    void End();
};

}

#endif /* HORUSEYE_GAME_SCENES_LOADING_H_ */
