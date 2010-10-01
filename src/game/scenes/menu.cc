#include <cstdlib>
#include <string>
#include "menu.h"
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../utils/levelloader.h"
#include "../utils/levelmanager.h"
#include "../scenes/imagescene.h"

namespace scene {

using namespace framework;
using namespace utils;

#define RECT_WIDTH          266
#define RECT_HEIGHT         90
#define SELECTION_WIDTH     864
#define SELECTION_HEIGHT    155
#define MENU_TOP            VIDEO_MANAGER()->video_size().y/3.0f
#define MENU_LEFT           VIDEO_MANAGER()->video_size().x/2.0f - RECT_WIDTH/2.0f
#define MENU_BOTTOM         MENU_TOP + Menu::MAIN_SELECT_NUM*RECT_HEIGHT
#define MENU_RIGHT          VIDEO_MANAGER()->video_size().x/2.0f + RECT_WIDTH/2.0f


Menu::Menu (int selection_num)
    : selection_(0) {

    selection_num_ = selection_num;
    handler_ = NULL;
    content_box_defined_ = false;
    selection_sprite_ = NULL;
    options_sprite_ = static_cast<Sprite**>(malloc(selection_num*sizeof(Sprite*)));
    selection_pos_ = new Vector2D[selection_num_];
    AddLayer(new Layer);

}


Menu::~Menu () {
    handler_->CleanUp();
    delete[] selection_pos_;
    free(options_sprite_);
}

void Menu::Update(float delta_t) {

    Scene::Update(delta_t);
    InputManager *input = Engine::reference()->input_manager();
    Vector2D mouse_pos = input->GetMousePosition();

    if (input->KeyPressed(K_ESCAPE)) {
        Finish();
        return;
    }

    if (input->KeyPressed(K_UP)) {
        selection_ = selection_ - 1 < 0 ? selection_num_ - 1 : selection_ - 1;
    }
    if (input->KeyPressed(K_DOWN))
        selection_++;
        selection_ = selection_ % selection_num_;

    bool on_selection = CheckMouse(mouse_pos);
    set_visible(true);

    Select();

    if (input->KeyPressed(K_RETURN) ||
        (on_selection && input->MouseUp(M_BUTTON_LEFT)))
        handler_->Handle(selection_);
}

void Menu::set_content_box(framework::Frame content_box) {
    content_box_ = content_box;
    content_box_defined_ = true;
    DecideWhereOptionsGo();
}
void Menu::set_selection_sprite(framework::Sprite *sprite) {
    selection_sprite_ = sprite;
    (*layers_.begin())->AddSprite(sprite);
    sprite->set_zindex(0.0f);
    InitialSelection();
}
void Menu::set_option_sprite(int index, framework::Sprite *sprite) {
    if (index >= 0 && index < selection_num_ && content_box_defined_) {
        options_sprite_[index] = sprite;
        (*layers_.begin())->AddSprite(sprite);
        sprite->set_zindex(10.0f);
        float selection_height = content_box_.height()/selection_num_,
              selection_width = content_box_.right()-content_box_.left();
        framework::Vector2D offset(
                (selection_width - sprite->image()->width())/2.0f,
                (selection_height - sprite->image()->height())/2.0f
        );
        sprite->set_position(selection_pos_[index] + offset);

    }
}
void Menu::AddSprite(framework::Sprite *sprite, framework::Vector2D pos) {
    (*layers_.begin())->AddSprite(sprite);
    sprite->set_position(pos);
    sprite->set_zindex(-10.0f);
}

void Menu::DecideWhereOptionsGo() {
    float selection_height = content_box_.height()/selection_num_, y;
    for (int i = 0; i < selection_num_; ++i) {
        y = content_box_.top() + static_cast<float>(i)*selection_height;
        selection_pos_[i] = Vector2D(content_box_.left(), y);
    }
    InitialSelection();
}

void Menu::InitialSelection() {
    if (selection_sprite_ && content_box_defined_) {
        float selection_height = content_box_.height()/selection_num_,
              selection_width = content_box_.right()-content_box_.left();
        framework::Vector2D offset(
                (selection_width - selection_sprite_->image()->width())/2.0f,
                (selection_height - selection_sprite_->image()->height())/2.0f
        );
        selection_sprite_->set_position(selection_pos_[0] + offset);
    }
}

bool Menu::CheckMouse (framework::Vector2D &mouse_pos) {

    static float    old_x = 0, old_y = 0;
    float           x = mouse_pos.x,
                    y = mouse_pos.y,
                    dx = x - old_x,
                    dy = y - old_y;
    static bool     on_selection = false;
    float selection_height = content_box_.height()/selection_num_;

    if (dx*dx > 0 || dy*dy > 0 || !visible_) {
        old_x = x;
        old_y = y;
        if ((y >= content_box_.top() && y < content_box_.bottom()) &&
            (x >= content_box_.left() && x < content_box_.right())) {
            on_selection = true;
            selection_ = (int)((y - content_box_.top())/selection_height);
        }
        else on_selection = false;
    }

    return on_selection;
}

void Menu::Select () {
    selection_sprite_->set_position(selection_pos_[selection_]);
}

}
