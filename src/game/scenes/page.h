#ifndef HORUSEYE_GAME_SCENES_PAGE_H_
#define HORUSEYE_GAME_SCENES_PAGE_H_

#include "../../framework/scene.h"
#include "../../framework/frame.h"
#include "menuhandler.h"
#include "menu.h"
#include "multipagemenu.h"

namespace framework {
class Sprite;
class Image;
}

namespace scene {

class Page : public Menu {
  public:

    Page (int selection_num, MultiPageMenu *manager);
    virtual ~Page ();

    void Update (float delta_t);

  private:

    MultiPageMenu *manager_;

};

}

#endif /* PAGE_H_ */
