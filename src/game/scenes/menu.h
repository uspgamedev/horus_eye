#ifndef HORUSEYE_GAME_SCENES_MENU_H_
#define HORUSEYE_GAME_SCENES_MENU_H_

#include <ugdk/action/scene.h>
#include <ugdk/graphic/node.h>
#include <ugdk/math/frame.h>
#include "menuhandler.h"

#define ALIGNMENT_LEFT   (-1)
#define ALIGNMENT_CENTER  0
#define ALIGNMENT_RIGHT   1

namespace ugdk {
class Sprite;
class Image;
class Layer;
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
    void set_selection_sprite(ugdk::Drawable *drawable);
    void set_selection_sprite(ugdk::Drawable *drawable[]);
    void set_option_sprite(int index, ugdk::Drawable *draw);
    void AddDrawable(ugdk::Drawable *drawable, ugdk::Vector2D pos);

    void Hide();
    void Show();
    void Toggle();

    const static float OPTION_ZINDEX;

  protected:

    void DecideWhereOptionsGo(int alignment);
    void InitialSelection();

    bool CheckMouse (ugdk::Vector2D &mouse_pos);
    void Select ();

    bool content_box_defined_;
    int selection_, selection_num_;
    MenuHandler *handler_;
    ugdk::Node *selection_sprite_[2];
    ugdk::Node **options_sprite_;
    ugdk::Vector2D *selection_pos_;
    ugdk::Frame    content_box_;
    ugdk::Node  *interface_node_;

};

}

#endif /* MENU_H_ */
