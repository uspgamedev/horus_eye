#ifndef HORUSEYE_GAME_SCENES_LOADING_H_
#define HORUSEYE_GAME_SCENES_LOADING_H_

#include <ugdk/action/scene.h>

namespace scene {

class Loading: public ugdk::Scene {
  typedef ugdk::Scene super;
  public:
    Loading();
    virtual ~Loading();
    virtual void Update(double delta_t);

    /// When the Loading gains focus, it becomes visible.
    virtual void Focus();

    /// When the Loading loses focus, it becomes invisible.
    virtual void DeFocus();

  protected:
    bool has_been_drawn_;
    ugdk::graphic::Node *loading_;
};

}

#endif /* HORUSEYE_GAME_SCENES_LOADING_H_ */
