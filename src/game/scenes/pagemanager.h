#ifndef HORUSEYE_GAME_SCENES_MULTIPAGEMENU_H_
#define HORUSEYE_GAME_SCENES_MULTIPAGEMENU_H_

#include <vector>
#include "../../framework/frame.h"
#include "menuhandler.h"
#include "menu.h"
using namespace std;

namespace ugdk {
class Sprite;
class Image;
}

namespace scene {

class PageManager : public Menu {
  public:
    typedef enum {
        START_PAGE=0,
        EXIT_PAGE,
        LEFT_PAGE,
        RIGHT_PAGE,
        WAITING
    }FinishState;

    PageManager (int page_num);
    virtual ~PageManager ();

    void Update (float delta_t);
    void setFinishState(int state);

  private:

    int page_num_, page_, state_;

};

}

#endif /* MULTIPAGEMENU_H_ */
