/*
 * menubuilder.cc
 *
 *  Created on: Sep 10, 2010
 *      Author: kazuo
 */

#include "menubuilder.h"
#include "../../framework/engine.h"
#include "../../framework/textmanager.h"
#include "../utils/levelmanager.h"
#include "world.h"
#include "menuhandler.h"
#include "menu.h"

namespace scene {

using namespace framework;
using namespace utils;

#define RECT_WIDTH              266
#define RECT_HEIGHT             90
#define SELECTION_WIDTH         864
#define SELECTION_HEIGHT        155
#define MENU_TOP                VIDEO_MANAGER()->video_size().y/2.3f
#define MENU_LEFT               VIDEO_MANAGER()->video_size().x/2.0f - RECT_WIDTH/2.0f
#define MENU_BOTTOM             MENU_TOP + MenuBuilder::MAIN_SELECT_NUM*RECT_HEIGHT
#define MENU_RIGHT              VIDEO_MANAGER()->video_size().x/2.0f + RECT_WIDTH/2.0f
#define PAUSE_TOP               VIDEO_MANAGER()->video_size().y/2.0f
#define PAUSE_LEFT              VIDEO_MANAGER()->video_size().x/2.0f - RECT_WIDTH/2.0f
#define PAUSE_BOTTOM            MENU_TOP + MenuBuilder::PAUSE_SELECT_NUM*RECT_HEIGHT*1.5f
#define PAUSE_RIGHT             VIDEO_MANAGER()->video_size().x/2.0f + RECT_WIDTH/2.0f

Menu *MenuBuilder::BuildMainMenu () {

    // Our main menu.
    Menu *menu = new Menu(MenuBuilder::MAIN_SELECT_NUM);

    // Setting its handler.
    menu->set_handler(new MainMenuHandler(menu));

    // The menu's content box.
    menu->set_content_box(Frame(MENU_LEFT, MENU_TOP, MENU_RIGHT, MENU_BOTTOM));

    // Setting the selection sprite.
    Sprite *selection_sprite = new Sprite;
    selection_sprite->Initialize(VIDEO_MANAGER()->LoadImage("data/images/selection.png"));
    selection_sprite->set_hotspot(Vector2D((SELECTION_WIDTH-RECT_WIDTH)/2.0f,( SELECTION_HEIGHT-RECT_HEIGHT)/2.0f));
    menu->set_selection_sprite(selection_sprite);

    // The game logo.
    Sprite *logo = new Sprite;
    logo->Initialize(VIDEO_MANAGER()->LoadImage("data/images/logo_560x334_black.png"));
    logo->set_hotspot(Vector2D(logo->image()->width() * 0.5f, 0));
    menu->AddSprite(logo, Vector2D(VIDEO_MANAGER()->video_size().x/2.0f, 0.0f));

    // The sprite of each option.
    for (int i = 0; i < MenuBuilder::MAIN_SELECT_NUM; ++i) {
        Sprite *options_sprite = new Sprite;
        switch (i) {
        case MenuBuilder::MAIN_SELECT_PLAY:
            options_sprite->Initialize(VIDEO_MANAGER()->LoadImage("data/images/play.png"));
            break;
        case MenuBuilder::MAIN_SELECT_HELP:
            options_sprite->Initialize(VIDEO_MANAGER()->LoadImage("data/images/help.png"));
            break;
        case MenuBuilder::MAIN_SELECT_SETTINGS:
            options_sprite->Initialize(VIDEO_MANAGER()->LoadImage("data/images/settings.png"));
            break;
        case MenuBuilder::MAIN_SELECT_ABOUT:
            options_sprite->Initialize(VIDEO_MANAGER()->LoadImage("data/images/credits.png"));
            break;
        case MenuBuilder::MAIN_SELECT_EXIT:
            options_sprite->Initialize(VIDEO_MANAGER()->LoadImage("data/images/exit.png"));
            break;
        }
        menu->set_option_sprite(i, options_sprite);
    }

    return menu;
}

void MenuBuilder::MainMenuHandler::Handle(int selection) {
    switch (selection) {
        case MenuBuilder::MAIN_SELECT_PLAY: {
            menu_->set_visible(false);
            LevelManager::reference()->ShowIntro();
            break;
        }
        case MenuBuilder::MAIN_SELECT_HELP: {
            MenuBuilder builder;
            Engine::reference()->PushScene(builder.BuildHelpMenu());
            menu_->set_visible(false);
            break;
        }
        case MenuBuilder::MAIN_SELECT_SETTINGS: {
            break;
        }
        case MenuBuilder::MAIN_SELECT_ABOUT: {
            menu_->set_visible(false);
            LevelManager::reference()->ShowCredits();
            break;
        }
        case MenuBuilder::MAIN_SELECT_EXIT: {
            Engine::reference()->quit();
            break;
        }
        default: {
            break;
        }
    }
}

Menu *MenuBuilder::BuildPauseMenu () {

    // Our main menu.
    Menu *menu = new Menu(MenuBuilder::PAUSE_SELECT_NUM);

    // The bg image.
    Image *bg_img = new Image;
    bg_img->Create(VIDEO_MANAGER()->video_size(), SDL_HWSURFACE);
    bg_img->Clear(0x808080);
    bg_img->SetAlpha(128);

    // Setting its handler.
    menu->set_handler(new PauseMenuHandler(menu, bg_img));

    // The menu's content box.
    menu->set_content_box(Frame(PAUSE_LEFT, PAUSE_TOP, PAUSE_RIGHT, PAUSE_BOTTOM));

    // Setting the selection sprite.
    Sprite *selection_sprite = new Sprite;
    selection_sprite->Initialize(VIDEO_MANAGER()->LoadImage("data/images/selection.png"));
    selection_sprite->set_hotspot(Vector2D((SELECTION_WIDTH-RECT_WIDTH)/2.0f,( SELECTION_HEIGHT-RECT_HEIGHT)/2.0f));
    menu->set_selection_sprite(selection_sprite);

    // The pause bg sprite.
    Sprite *bg = new Sprite;;
    bg->Initialize(bg_img);
    menu->AddSprite(bg, Vector2D());

    // The sprite of each option.
    for (int i = 0; i < MenuBuilder::MAIN_SELECT_NUM; ++i) {
        Sprite *options_sprite = new Sprite;
        TEXT_MANAGER()->setFont("data/font/Filmcrypob.ttf", 70, NULL);
        switch (i) {
        case MenuBuilder::PAUSE_SELECT_CONTINUE:
            options_sprite->Initialize(TEXT_MANAGER()->LoadFancyLine("Continue"));
            break;
        case MenuBuilder::PAUSE_SELECT_EXIT_GAME:
            options_sprite->Initialize(TEXT_MANAGER()->LoadFancyLine("Exit"));
            break;
        }
        menu->set_option_sprite(i, options_sprite);
    }

    return menu;

}

void MenuBuilder::PauseMenuHandler::Handle(int selection) {
    switch (selection) {
        case MenuBuilder::PAUSE_SELECT_CONTINUE: {
            menu_->Finish();
            break;
        }
        case MenuBuilder::PAUSE_SELECT_EXIT_GAME: {
            menu_->Finish();
            WORLD()->FinishLevel(LevelManager::FINISH_QUIT);
            break;
        }
        default: {
            break;
        }
    }
}

void MenuBuilder::PauseMenuHandler::CleanUp() {
    bg_img_->Destroy();
    delete bg_img_;
}

Menu *MenuBuilder::BuildHelpMenu () {

    // Our main menu.
    Menu *menu = new Menu(1);

    // Setting its handler.
    menu->set_handler(new HelpMenuHandler(menu));

    Image* img = VIDEO_MANAGER()->LoadImage("data/images/exit.png");

    // The menu's content box.
    float top = VIDEO_MANAGER()->video_size().y - img->height();
    menu->set_content_box(Frame(MENU_LEFT, top, MENU_RIGHT, VIDEO_MANAGER()->video_size().y));

    // Setting the selection sprite.
    Sprite *selection_sprite = new Sprite;
    selection_sprite->Initialize(VIDEO_MANAGER()->LoadImage("data/images/selection.png"));
    selection_sprite->set_hotspot(Vector2D((SELECTION_WIDTH-RECT_WIDTH)/2.0f,( SELECTION_HEIGHT-RECT_HEIGHT)/2.0f));
    menu->set_selection_sprite(selection_sprite);
    // The game logo.//
    Sprite *logo = new Sprite;

    logo->Initialize(VIDEO_MANAGER()->LoadImage("data/images/horus_tutorial_ee.png"));
    logo->set_hotspot(Vector2D(logo->image()->width() * 0.5f, 0));
    menu->AddSprite(logo, Vector2D(VIDEO_MANAGER()->video_size().x/2.0f, 0.0f));

    Sprite *options_sprite = new Sprite;
    options_sprite->Initialize(img);
    menu->set_option_sprite(0, options_sprite);

    return menu;
}

void MenuBuilder::HelpMenuHandler::Handle(int selection) {
    menu_->Finish();
}

}
