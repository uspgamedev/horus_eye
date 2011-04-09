#ifndef HORUSEYE_GAME_SCENES_PAGE_H_
#define HORUSEYE_GAME_SCENES_PAGE_H_

#include "../../framework/scene.h"
#include "../../framework/frame.h"
#include "menuhandler.h"
#include "menu.h"
#include "pagemanager.h"

namespace framework {
class Sprite;
class Image;
}

namespace scene {

class Page : public Menu {
  public:
    typedef enum {
        FIRST_PAGE=0,
        LAST_PAGE,
        NORMAL_PAGE,
        SOLO_PAGE
    } PageTypes;

    Page (int selection_num, int page_num, PageManager *manager);
    virtual ~Page ();

    void Update (float delta_t);
    int CheckMouse (framework::Vector2D &mouse_pos);

  private:

    PageManager *manager_;
    int page_type_;

};

}

#endif /* PAGE_H_ */
