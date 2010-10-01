#include <cstdlib>
#include <string>
#include "multipagemenu.h"
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
#define MENU_BOTTOM         MENU_TOP + MultiPageMenu::MAIN_SELECT_NUM*RECT_HEIGHT
#define MENU_RIGHT          VIDEO_MANAGER()->video_size().x/2.0f + RECT_WIDTH/2.0f


MultiPageMenu::MultiPageMenu (int selection_num, int page_num)
    : Menu(selection_num) {

    page_num_ = page_num;
}

MultiPageMenu::~MultiPageMenu () {

}

void MultiPageMenu::Update(float delta_t) {

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
    if (input->KeyPressed(K_DOWN)){
        selection_++;
        selection_ = selection_ % selection_num_;
    }
    if(input->KeyPressed(K_LEFT)){
        printf("Left Pressed\n");
    }
    if(input->KeyPressed(K_RIGHT)){
        printf("Right Pressed\n");
    }

    bool on_selection = CheckMouse(mouse_pos);
    set_visible(true);

    Select();

    if (input->KeyPressed(K_RETURN) ||
        (on_selection && input->MouseUp(M_BUTTON_LEFT)))
        handler_->Handle(selection_);
}

}
