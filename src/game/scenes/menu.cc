//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/menu.h
// Definicao da classe Menu.
//

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

    set_visible(true);

    if (input->KeyPressed(K_ESCAPE)) {
        Engine::reference()->quit();
        return;
    }

    if (input->KeyPressed(K_UP)) {
        selection_ = selection_ - 1 < 0 ? selection_num_ - 1 : selection_ - 1;
    }
    if (input->KeyPressed(K_DOWN))
        selection_ = (++selection_)%selection_num_;

    bool on_selection = CheckMouse(mouse_pos);

    Select();

    if (input->KeyPressed(K_RETURN) ||
        (on_selection && input->MouseUp(M_BUTTON_LEFT)))
        handler_->Handle(selection_);

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
    if (selection_sprite_ && content_box_defined_)
        selection_sprite_->set_position(selection_pos_[0]);
}

bool Menu::CheckMouse (framework::Vector2D &mouse_pos) {

    static float    old_x = 0, old_y = 0;
    float           x = mouse_pos.x,
                    y = mouse_pos.y,
                    dx = x - old_x,
                    dy = y - old_y;
    static bool     on_selection = false;
    float selection_height = content_box_.height()/selection_num_;

    if (dx*dx > 0 || dy*dy > 0) {
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
