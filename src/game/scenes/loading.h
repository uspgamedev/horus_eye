#ifndef HORUSEYE_GAME_SCENES_LOADING_H_
#define HORUSEYE_GAME_SCENES_LOADING_H_

#include "../../framework/scene.h"

namespace scene {

class Loading: public ugdk::Scene {
  public:
    Loading();
    virtual ~Loading();
    virtual void Update(float delta_t);

  protected:
    bool has_been_drawn_;
};

}

#endif /* HORUSEYE_GAME_SCENES_LOADING_H_ */
