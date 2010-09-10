

#ifndef HORUSEYE_GAME_SCENES_MENUBUILDER_H_
#define HORUSEYE_GAME_SCENES_MENUBUILDER_H_

#include "menuhandler.h"

namespace framework {
class Image;
}

namespace scene {

class Menu;

class MenuBuilder {
  public:
    MenuBuilder () {}
    virtual ~MenuBuilder () {}
    Menu *BuildMainMenu();
    Menu *BuildPauseMenu();
  private:
    typedef enum {
        MAIN_SELECT_PLAY = 0,
        MAIN_SELECT_HELP,
        MAIN_SELECT_SETTINGS,
        MAIN_SELECT_ABOUT,
        MAIN_SELECT_EXIT,
        MAIN_SELECT_NUM
    } MainMenuSelection;
    typedef enum {
        PAUSE_SELECT_CONTINUE = 0,
        PAUSE_SELECT_EXIT_GAME,
        PAUSE_SELECT_NUM
    } PauseMenuSelection;
    class MainMenuHandler : public MenuHandler {
      public:
        MainMenuHandler(Menu *menu) : MenuHandler(menu) {}
        ~MainMenuHandler() {}
        void Handle(int selection);
        void CleanUp() {}
    };
    class PauseMenuHandler : public MenuHandler {
      public:
        PauseMenuHandler(Menu *menu, framework::Image *bg_img)
            : MenuHandler(menu), bg_img_(bg_img) {}
        ~PauseMenuHandler() {}
        void Handle(int selection);
        void CleanUp();
      private:
        framework::Image *bg_img_;
    };
};

}

#endif /* HORUSEYE_GAME_SCENES_MENUBUILDER_H_ */
