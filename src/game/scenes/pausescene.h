#ifndef PAUSESCENE_H_
#define PAUSESCENE_H_

#include "../../framework/scene.h"

namespace framework {
class Image;
}

namespace scene {

class PauseScene: public framework::Scene {
  public:
    PauseScene ();
    virtual ~PauseScene ();

    void Update (float delta_t);
    void End ();

  private:
    framework::Image *image_;
};

}

#endif /* PAUSESCENE_H_ */
