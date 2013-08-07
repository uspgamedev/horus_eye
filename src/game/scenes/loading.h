#ifndef HORUSEYE_GAME_SCENES_LOADING_H_
#define HORUSEYE_GAME_SCENES_LOADING_H_

#include <ugdk/graphic.h>
#include <ugdk/action/scene.h>

namespace scene {

class Loading: public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    Loading();
    virtual ~Loading();

    /// When the Loading gains focus, it becomes visible.
    virtual void Focus();

    /// When the Loading loses focus, it becomes invisible.
    virtual void DeFocus();

  private:
    ugdk::graphic::Node* loading_;
};

}

#endif /* HORUSEYE_GAME_SCENES_LOADING_H_ */
