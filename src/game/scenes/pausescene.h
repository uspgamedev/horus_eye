#ifndef PAUSESCENE_H_
#define PAUSESCENE_H_

#include "../../framework/scene.h"

namespace ugdk {
class Image;
}

namespace scene {

class PauseScene: public ugdk::Scene {
  public:
    PauseScene ();
    virtual ~PauseScene ();

    void Update (float delta_t);
    void End ();

  private:
    ugdk::Image *image_;
};

}

#endif /* PAUSESCENE_H_ */
