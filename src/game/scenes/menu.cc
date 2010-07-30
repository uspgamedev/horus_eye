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

Menu::Menu () : selection_(SELECT_PLAY) {

    select_rect_ = new Sprite;
    rect_ = new Image;
    rect_->Create(Vector2D(200,50));
    rect_->Clear(0x000000FF);
    select_rect_->Initialize(rect_);

    Layer *layer = new Layer;
    AddLayer(layer);

    layer->AddSprite(select_rect_);

    for (int y, i = 0; i < Menu::SELECT_NUM; ++i) {
        y = 768/2 + i*50;
        select_pos_[i] = Vector2D(412, y);
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
        (on_selection && input->MouseDown(M_BUTTON_LEFT)))
        Choose();

}

bool Menu::CheckMouse (framework::Vector2D &mouse_pos) {
    return false;
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
