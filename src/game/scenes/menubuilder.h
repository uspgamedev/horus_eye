

#ifndef HORUSEYE_GAME_SCENES_MENUBUILDER_H_
#define HORUSEYE_GAME_SCENES_MENUBUILDER_H_

#include "menuhandler.h"
#include <vector>
#include "../../framework/sprite.h"

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
    Menu *BuildHelpMenu();
    Menu *BuildSettingsMenu();
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
    typedef enum {
        SETTINGS_SELECT_RESOLUTION = 0,
        SETTINGS_SELECT_FULLSCREEN,
        SETTINGS_SELECT_MUSIC,
        SETTINGS_SELECT_SOUNDS,
        SETTINGS_SELECT_LANGUAGE,
        SETTINGS_SELECT_EXIT,
        SETTINGS_SELECT_NUM
    } SettingsMenuSelection;
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
    class HelpMenuHandler : public MenuHandler {
      public:
        HelpMenuHandler(Menu *menu) : MenuHandler(menu) {}
        ~HelpMenuHandler() {}
        void Handle(int selection);
        void CleanUp() {}
    };
    class SettingsMenuHandler : public MenuHandler {
      public:
        SettingsMenuHandler(Menu *menu) : MenuHandler(menu) {}
        ~SettingsMenuHandler() {}
        void Handle(int selection);
        void CleanUp();
        void BuildSprites();
      private:  
        static std::string     resolution_[12], settings_[6], on_off_[2], language_[2];
        static int             sprites_active_[5];
        framework::Image *settings_images_[10], *resolution_images_[12], *on_off_images_[3][2], *language_images_[2];
        framework::Sprite *settings_sprites_[10], *resolution_sprites_[12], *on_off_sprites_[3][2], *language_sprites_[2];
    };
};

}

#endif /* HORUSEYE_GAME_SCENES_MENUBUILDER_H_ */
