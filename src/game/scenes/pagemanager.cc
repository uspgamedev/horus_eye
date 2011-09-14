#include <cstdlib>
#include <string>
#include "pagemanager.h"
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/action/sprite.h>
#include <ugdk/action/animation.h>
#include "../utils/levelloader.h"
#include "../utils/levelmanager.h"
#include "../scenes/imagescene.h"

namespace scene {

using namespace ugdk;
using namespace utils;

#define RECT_WIDTH          266
#define RECT_HEIGHT         90
#define SELECTION_WIDTH     864
#define SELECTION_HEIGHT    155
#define MENU_TOP            VIDEO_MANAGER()->video_size().y/3.0f
#define MENU_LEFT           VIDEO_MANAGER()->video_size().x/2.0f - RECT_WIDTH/2.0f
#define MENU_BOTTOM         MENU_TOP + PageManager::MAIN_SELECT_NUM*RECT_HEIGHT
#define MENU_RIGHT          VIDEO_MANAGER()->video_size().x/2.0f + RECT_WIDTH/2.0f


PageManager::PageManager (int page_num)
    : Menu(0){

    page_num_ = page_num;
    page_ = 0;
    state_ = START_PAGE;
}

PageManager::~PageManager () {

}

void PageManager::Update(float delta_t) {

    Scene::Update(delta_t);
    InputManager *input = Engine::reference()->input_manager();
    Vector2D mouse_pos = input->GetMousePosition();
    set_visible(true);

    switch (state_){
        //Handles which page to show
        case PageManager::START_PAGE: {
            handler_->Handle(0);
            break;
        }
        case PageManager::EXIT_PAGE: {
            Finish();
            break;
        }
        case PageManager::LEFT_PAGE: {
            page_--;
            state_ = WAITING;
            //Defense against page badly declared
            if(page_<0){
                printf("Pages exploded left %d of %d\n", page_, page_num_);
                Finish();
            }else
                handler_->Handle(page_);
            break;
        }
        case PageManager::RIGHT_PAGE: {
            page_++;
            state_ = WAITING;
            //Defense against page badly declared
            if(page_>=page_num_){
                printf("Pages exploded right %d of %d\n", page_, page_num_);
                Finish();
            }else
                handler_->Handle(page_);
            break;
        }
        case PageManager::WAITING: {
            break;
        }
        default: {
            break;
        }
    }
}

void PageManager::setFinishState(int state){
    state_ = state;
}

}
