#ifndef HORUSEYE_GAME_SCENES_MENU_H_
#define HORUSEYE_GAME_SCENES_MENU_H_

#include "../../framework/scene.h"
#include "../../framework/frame.h"
#include "menuhandler.h"

#define ALIGNMENT_LEFT   (-1)
#define ALIGNMENT_CENTER  0
#define ALIGNMENT_RIGHT   1

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

    void set_handler(MenuHandler* handler);
    void set_content_box(framework::Frame content_box);

    void set_content_box(framework::Frame content_box, int alignment);
    void set_selection_sprite(framework::Sprite *sprite);
    void set_selection_sprite(framework::Sprite *sprite[]);
    void set_option_sprite(int index, framework::Sprite *sprite);
    void AddSprite(framework::Sprite *sprite, framework::Vector2D pos);

    const static float OPTION_ZINDEX;

  protected:

    void DecideWhereOptionsGo(int alignment);
    void InitialSelection();

    bool CheckMouse (framework::Vector2D &mouse_pos);
    void Select ();

    bool content_box_defined_;
    int selection_, selection_num_;
    MenuHandler *handler_;
    framework::Sprite *selection_sprite_[2];
    framework::Sprite **options_sprite_;
    framework::Vector2D *selection_pos_;
    framework::Frame    content_box_;

};

}

#endif /* MENU_H_ */
