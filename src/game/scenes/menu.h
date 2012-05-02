#ifndef HORUSEYE_GAME_SCENES_MENU_H_
#define HORUSEYE_GAME_SCENES_MENU_H_

#include <ugdk/action/scene.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/math/frame.h>
#include "menuhandler.h"

namespace scene {

class Menu: public ugdk::action::Scene {
  public:

    Menu (int selection_num);
    virtual ~Menu ();

    void Update (double delta_t);

    void set_handler(MenuHandler* handler);
    void set_content_box(ugdk::Frame content_box);

    const ugdk::Vector2D& get_selection_position(int index) const {
        return selection_pos_[index];
    }

    void set_content_box(ugdk::Frame content_box, ugdk::graphic::Drawable::HookPoint alignment);
    void set_selection_sprite(ugdk::graphic::Drawable *drawable);
    void set_selection_sprite(ugdk::graphic::Drawable **drawable);
    void set_option_sprite(int index, ugdk::graphic::Drawable *draw);
    void AddDrawable(ugdk::graphic::Drawable *drawable, ugdk::Vector2D pos);
    void AddNode(ugdk::graphic::Node *node);

	void Hide()   { set_visibility(    false); }
    void Show()   { set_visibility(     true); }
    void Toggle() { set_visibility(!visible_); }

    const static double OPTION_ZINDEX;

  protected:
	void set_visibility(const bool visibility);
    void DecideWhereOptionsGo(ugdk::graphic::Drawable::HookPoint alignment);

    bool CheckMouse (ugdk::Vector2D &mouse_pos);
    void Select ();

	ugdk::graphic::Drawable::HookPoint option_alignment_;

	bool visible_;

    bool content_box_defined_;

    /// Currently selected option
    int selection_;

    /// Number of selection options
    int selection_num_;


    MenuHandler *handler_;

    ugdk::graphic::Node *selection_node_[2];
    ugdk::graphic::Node **options_node_;
    ugdk::Vector2D *selection_pos_;
    ugdk::Frame    content_box_;

};

}

#endif /* MENU_H_ */
