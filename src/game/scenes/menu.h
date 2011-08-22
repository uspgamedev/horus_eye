#ifndef HORUSEYE_GAME_SCENES_MENU_H_
#define HORUSEYE_GAME_SCENES_MENU_H_

#include "ugdk/scene.h"
#include "ugdk/frame.h"
#include "menuhandler.h"

#define ALIGNMENT_LEFT   (-1)
#define ALIGNMENT_CENTER  0
#define ALIGNMENT_RIGHT   1

namespace ugdk {
class Sprite;
class Image;
}

namespace scene {

class Menu: public ugdk::Scene {
  public:

    Menu (int selection_num);
    virtual ~Menu ();

    void Update (float delta_t);

    void set_handler(MenuHandler* handler);
    void set_content_box(ugdk::Frame content_box);

    void set_content_box(ugdk::Frame content_box, int alignment);
    void set_selection_sprite(ugdk::Sprite *sprite);
    void set_selection_sprite(ugdk::Sprite *sprite[]);
    void set_option_sprite(int index, ugdk::Sprite *sprite);
    void AddSprite(ugdk::Sprite *sprite, ugdk::Vector2D pos);

    const static float OPTION_ZINDEX;

  protected:

    void DecideWhereOptionsGo(int alignment);
    void InitialSelection();

    bool CheckMouse (ugdk::Vector2D &mouse_pos);
    void Select ();

    bool content_box_defined_;
    int selection_, selection_num_;
    MenuHandler *handler_;
    ugdk::Sprite *selection_sprite_[2];
    ugdk::Sprite **options_sprite_;
    ugdk::Vector2D *selection_pos_;
    ugdk::Frame    content_box_;

};

}

#endif /* MENU_H_ */
