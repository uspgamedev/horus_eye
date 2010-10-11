

#ifndef HORUSEYE_GAME_SCENES_MENUHANDLER_H_
#define HORUSEYE_GAME_SCENES_MENUHANDLER_H_

namespace scene {

class Menu;

// A MenuHandler is what decides what happens when the user presses enter
// or clicks on an option of a Menu.
class MenuHandler {
  public:
    virtual ~MenuHandler () {}

    virtual void Handle (int selection, int modifier = 0) = 0;
    virtual void CleanUp () = 0;

  protected:
    MenuHandler (Menu *menu) : menu_(menu) {}
    Menu *menu_;
};

}

#endif /* HORUSEYE_GAME_SCENES_MENUHANDLER_H_ */
