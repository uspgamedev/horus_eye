//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/menu.h
// Definicao da classe Menu.
//

#include "menu.h"
#include "../../framework/engine.h"
#include "../../framework/inputmanager.h"
#include "../utils/levelloader.h"
#include "../scenes/imagescene.h"

namespace scene {

using namespace framework;
using namespace utils;

#define RECT_WIDTH  200
#define RECT_HEIGHT 50
#define MENU_TOP    VIDEO_MANAGER()->video_size().y/2.0
#define MENU_LEFT   VIDEO_MANAGER()->video_size().x/2.0 - RECT_WIDTH/2.0
#define MENU_BOTTOM MENU_TOP + Menu::SELECT_NUM*RECT_HEIGHT
#define MENU_RIGHT  VIDEO_MANAGER()->video_size().x/2.0 + RECT_WIDTH/2.0


Menu::Menu () : selection_(SELECT_PLAY) {

    select_rect_ = new Sprite;
    rect_ = new Image;
    rect_->Create(Vector2D(RECT_WIDTH, RECT_HEIGHT));
    rect_->Clear(0x000000FF);
    select_rect_->Initialize(rect_);

    Layer *layer = new Layer;
    AddLayer(layer);

    layer->AddSprite(select_rect_);

    for (int y, i = 0; i < Menu::SELECT_NUM; ++i) {
        y = MENU_TOP + i*RECT_HEIGHT;
        select_pos_[i] = Vector2D(MENU_LEFT, y);
    }

    select_rect_->set_position(select_pos_[Menu::SELECT_PLAY]);

}

Menu::~Menu () {
    rect_->Destroy();
    delete rect_;
}

Menu::Selection operator ++ (Menu::Selection& selection) {
    return selection = (Menu::Selection)((selection+1)%Menu::SELECT_NUM);
}

Menu::Selection operator -- (Menu::Selection& selection) {
    selection = (Menu::Selection)((selection-1)%Menu::SELECT_NUM);
    if (selection < 0) selection = (Menu::Selection)(Menu::SELECT_NUM-1);
    return selection;
}

void Menu::Update(float delta_t) {

    InputManager *input = Engine::reference()->input_manager();
    Vector2D mouse_pos = input->GetMousePosition();

    set_visible(true);

    if (input->KeyPressed(K_ESCAPE)) {
        Engine::reference()->quit();
        return;
    }

    if (input->KeyPressed(K_UP))
        --selection_;
    if (input->KeyPressed(K_DOWN))
        ++selection_;

    bool on_selection = CheckMouse(mouse_pos);

    Select();

    if (input->KeyPressed(K_RETURN) ||
        (on_selection && input->MousePressed(M_BUTTON_LEFT)))
        Choose();

}

bool Menu::CheckMouse (framework::Vector2D &mouse_pos) {

    static float    old_x = 0, old_y = 0;
    float           x = mouse_pos.x,
                    y = mouse_pos.y,
                    dx = x - old_x,
                    dy = y - old_y;
    static bool     on_selection = false;

    if (dx*dx > 0 || dy*dy > 0) {
        old_x = x;
        old_y = y;
        if ((y >= MENU_TOP && y < MENU_BOTTOM) &&
            (x >= MENU_LEFT && x < MENU_RIGHT)) {
            on_selection = true;
            selection_ = (Menu::Selection)(((int)y - MENU_TOP)/RECT_HEIGHT);
        }
        else on_selection = false;
    }

    return on_selection;
}

void Menu::Select () {
    select_rect_->set_position(select_pos_[selection_]);
}

void Menu::Choose () {
    switch (selection_) {
        case Menu::SELECT_PLAY: {

            Engine *engine = Engine::reference();
            World *world = new World;
            LevelLoader *loader = new LevelLoader(world);
            loader->Load("data/levels/level_test.txt");
            engine->PushScene(world);
            world->set_visible(false);
            delete loader;

            Image *intro = VIDEO_MANAGER()->LoadImage("data/images/intro_text_en.png");
            engine->PushScene(new ImageScene(NULL, intro, 30));

            set_visible(false);

            break;
        }
        case Menu::SELECT_HELP: {
            break;
        }
        case Menu::SELECT_SETTINGS: {
            break;
        }
        case Menu::SELECT_ABOUT: {
            break;
        }
        case Menu::SELECT_EXIT: {
            Engine::reference()->quit();
            break;
        }
        default: {
            break;
        }
    }

}

}
