//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/menu.cc
// Definicao da classe Menu.
//

#ifndef HORUSEYE_GAME_SCENES_MENU_H_
#define HORUSEYE_GAME_SCENES_MENU_H_

#include "../../framework/scene.h"

namespace framework {
class Sprite;
class Image;
}

namespace scene {

class Menu: public framework::Scene {
  public:

    Menu ();
    virtual ~Menu ();

    void Update (float delta_t);

    typedef enum {
        SELECT_PLAY,
        SELECT_HELP,
        SELECT_SETTINGS,
        SELECT_ABOUT,
        SELECT_EXIT,
        SELECT_NUM
    } Selection;

  private:

    bool CheckMouse (framework::Vector2D &mouse_pos);
    void Select ();
    void Choose ();

    Selection selection_;
    framework::Sprite *select_rect_;
    framework::Image *rect_;
    framework::Vector2D select_pos_[SELECT_NUM];



};

}

#endif /* MENU_H_ */
