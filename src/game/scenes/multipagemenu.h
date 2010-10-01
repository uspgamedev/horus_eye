#ifndef HORUSEYE_GAME_SCENES_MULTIPAGEMENU_H_
#define HORUSEYE_GAME_SCENES_MULTIPAGEMENU_H_

#include "../../framework/scene.h"
#include "../../framework/frame.h"
#include "menuhandler.h"
#include "menu.h"

namespace framework {
class Sprite;
class Image;
}

namespace scene {

class MultiPageMenu : public Menu {
  public:

    MultiPageMenu (int selection_num, int page_num);
    virtual ~MultiPageMenu ();

    void Update (float delta_t);

  private:

    int page_num_, page_;

};

}

#endif /* MULTIPAGEMENU_H_ */
