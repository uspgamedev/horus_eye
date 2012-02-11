#ifndef PAUSESCENE_H_
#define PAUSESCENE_H_

#include <ugdk/action/scene.h>

namespace scene {

class PauseScene: public ugdk::Scene {
  public:
    PauseScene ();
    virtual ~PauseScene ();

    void Update (double delta_t);
};

}

#endif /* PAUSESCENE_H_ */
