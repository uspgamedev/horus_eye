#include <cstdlib>
#include <string>
#include "page.h"
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/animation.h"
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
#define ARROW_WIDTH     100
#define ARROW_HEIGHT    100
#define MENU_TOP            VIDEO_MANAGER()->video_size().y/3.0f
#define MENU_LEFT           VIDEO_MANAGER()->video_size().x/2.0f - RECT_WIDTH/2.0f
#define MENU_BOTTOM         MENU_TOP + Page::MAIN_SELECT_NUM*RECT_HEIGHT
#define MENU_RIGHT          VIDEO_MANAGER()->video_size().x/2.0f + RECT_WIDTH/2.0f

Page::Page (int selection_num, int page_type, PageManager *manager)
    : Menu(selection_num) {

    manager_ = manager;
    page_type_ = page_type;

    //Add arrows
    Image *arrow = VIDEO_MANAGER()->LoadImage("data/images/arrows.png");
    arrow->set_frame_size(Vector2D(arrow->width()/2.0, arrow->height()));

    if(page_type != Page::FIRST_PAGE && page_type_!=SOLO_PAGE){
        Sprite *left_arrow = new Sprite;
        left_arrow->Initialize(arrow);
        left_arrow->set_hotspot(Vector2D(0.0, arrow->height()));
        left_arrow->set_mirror(MIRROR_HFLIP);
        AddSprite(left_arrow, Vector2D(0.0, VIDEO_MANAGER()->video_size().y));
    }
    if(page_type != Page::LAST_PAGE && page_type_!=SOLO_PAGE){
        Sprite *right_arrow = new Sprite;
        right_arrow->Initialize(arrow);
        right_arrow->set_hotspot(Vector2D(arrow->width()/2.0, arrow->height()));
        AddSprite(right_arrow, VIDEO_MANAGER()->video_size());
    }

}

Page::~Page () {

}

void Page::Update(float delta_t) {

    Scene::Update(delta_t);
    InputManager *input = Engine::reference()->input_manager();
    Vector2D mouse_pos = input->GetMousePosition();
    int mouse_on = CheckMouse(mouse_pos);
    set_visible(true);
        
    Select();
    

    if (input->KeyPressed(K_ESCAPE)) {
        manager_->setFinishState(PageManager::EXIT_PAGE);
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
    if((page_type_!=FIRST_PAGE && page_type_!=SOLO_PAGE) && 
        (input->KeyPressed(K_LEFT) ||
        (mouse_on==1 && input->MouseUp(M_BUTTON_LEFT)))){
        manager_->setFinishState(PageManager::LEFT_PAGE);
        handler_->Handle(selection_num_);
    }
    if((page_type_!=LAST_PAGE && page_type_!=SOLO_PAGE) &&
        (input->KeyPressed(K_RIGHT) ||
        (mouse_on==2 && input->MouseUp(M_BUTTON_LEFT)))){
        manager_->setFinishState(PageManager::RIGHT_PAGE);
        handler_->Handle(selection_num_ + 1);
    }
    
    if (input->KeyPressed(K_RETURN) ||
        (mouse_on==3 && input->MouseUp(M_BUTTON_LEFT))){
        manager_->setFinishState(PageManager::EXIT_PAGE);
        handler_->Handle(selection_);
    }
}

// Verifica a posição do mouse
int Page::CheckMouse (framework::Vector2D &mouse_pos) {

    static float    old_x = 0, old_y = 0;
    float           x = mouse_pos.x,
                    y = mouse_pos.y,
                    dx = x - old_x,
                    dy = y - old_y;
    static int mouse_on = 0;
    float selection_height = content_box_.height()/selection_num_;
    Vector2D video_size = VIDEO_MANAGER()->video_size();

    if (dx*dx > 0 || dy*dy > 0 || !visible_) {
        old_x = x;
        old_y = y;
        if ((y >= content_box_.top() && y < content_box_.bottom()) &&
            (x >= content_box_.left() && x < content_box_.right())) {
            selection_ = (int)((y - content_box_.top())/selection_height);
            mouse_on=3;
        } else if ((y >= (video_size.y - ARROW_HEIGHT) && y < video_size.y) &&
                   (x >= 0 && x < ARROW_WIDTH)){
            mouse_on=1;
        } else if ((y >= (video_size.y - ARROW_HEIGHT) && y < video_size.y) &&
                 (x >= (video_size.x - ARROW_WIDTH) && x < video_size.x)){
            mouse_on=2;
        } else
            mouse_on=0;
    }

    return mouse_on;
}

}
