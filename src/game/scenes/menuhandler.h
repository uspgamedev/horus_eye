

#ifndef HORUSEYE_GAME_SCENES_MENUHANDLER_H_
#define HORUSEYE_GAME_SCENES_MENUHANDLER_H_

namespace scene {

class Menu;

class MenuHandler {
  public:
    virtual ~MenuHandler () {}

    virtual void Handle (int selection) = 0;
    virtual void CleanUp () = 0;

  protected:
    MenuHandler (Menu *menu) : menu_(menu) {}
    Menu *menu_;
};

}

#endif /* HORUSEYE_GAME_SCENES_MENUHANDLER_H_ */
