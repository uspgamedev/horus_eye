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
#include "../../framework/frame.h"
#include "menuhandler.h"

namespace framework {
class Sprite;
class Image;
}

namespace scene {

class Menu: public framework::Scene {
  public:

    Menu (int selection_num);
    virtual ~Menu ();

    void Update (float delta_t);

    void set_handler(MenuHandler* handler) {
        handler_ = handler;
    }
    void set_content_box(framework::Frame content_box) {
        content_box_ = content_box;
        content_box_defined_ = true;
        DecideWhereOptionsGo();
    }
    void set_selection_sprite(framework::Sprite *sprite) {
        selection_sprite_ = sprite;
        (*layers_.begin())->AddSprite(sprite);
        sprite->set_zindex(0.0f);
        InitialSelection();
    }
    void set_option_sprite(int index, framework::Sprite *sprite) {
        if (index >= 0 && index < selection_num_ && content_box_defined_) {
            options_sprite_[index] = sprite;
            (*layers_.begin())->AddSprite(sprite);
            sprite->set_zindex(10.0f);
            sprite->set_position(selection_pos_[index]);
        }
    }
    void AddSprite(framework::Sprite *sprite, framework::Vector2D pos) {
        (*layers_.begin())->AddSprite(sprite);
        sprite->set_position(pos);
        sprite->set_zindex(-10.0f);
    }

  private:

    void DecideWhereOptionsGo();
    void InitialSelection();

    bool CheckMouse (framework::Vector2D &mouse_pos);
    void Select ();

    bool                content_box_defined_;
    int                 selection_, selection_num_;
    MenuHandler         *handler_;
    framework::Sprite   *selection_sprite_,
                        **options_sprite_;
    framework::Vector2D *selection_pos_;
    framework::Frame    content_box_;

};

}

#endif /* MENU_H_ */
