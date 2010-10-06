#include "menubuilder.h"
#include "../../framework/engine.h"
#include "../../framework/textmanager.h"
#include "../utils/levelmanager.h"
#include "../utils/textloader.h"
#include "../utils/settings.h"
#include "world.h"
#include "menuhandler.h"
#include "menu.h"
#include "multipagemenu.h"

namespace scene {

using namespace framework;
using namespace utils;
using namespace std;

#define RECT_WIDTH              266
#define RECT_HEIGHT             90
#define SET_RECT_WIDTH          266
#define SET_RECT_HEIGHT         70
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
#define SET_TOP                 VIDEO_MANAGER()->video_size().y/4.0f + 120
#define SET_LEFT                VIDEO_MANAGER()->video_size().x/10.0f - SET_RECT_WIDTH/2.0f
#define SET_BOTTOM              MENU_TOP + MenuBuilder::SETTINGS_SELECT_NUM*SET_RECT_HEIGHT
#define SET_RIGHT               VIDEO_MANAGER()->video_size().x/2.0f + SET_RECT_WIDTH/2.0f
#define NUM_RESOL               12
#define NUM_ON_OFF              2

string  MenuBuilder::SettingsMenuHandler::resolution_[12] = {"800x600", "1024x768", "1280x720", 
                "1280x800", "1280x960", "1280x1024", 
                 "1366x768", "1440x900", "1600x900", 
                  "1600x1080", "1920x1080", "1920x1200"};
string  MenuBuilder::SettingsMenuHandler::settings_[6] = {"Resolution", "Fullscreen", "Music", 
                "Sound Effects", "Language", "Exit"};
string  MenuBuilder::SettingsMenuHandler::on_off_[2] = {"On", "Off" };
string  MenuBuilder::SettingsMenuHandler::language_[2] = {"English", "Portugues" };
int     MenuBuilder::SettingsMenuHandler::sprites_active_[5] = {0,0,0,0,0};

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
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Play"));
            break;
        case MenuBuilder::MAIN_SELECT_HELP:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Help"));
            break;
        case MenuBuilder::MAIN_SELECT_SETTINGS:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Settings"));
            break;
        case MenuBuilder::MAIN_SELECT_ABOUT:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Credits"));
            break;
        case MenuBuilder::MAIN_SELECT_EXIT:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Exit"));
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
            MenuBuilder builder;
            Engine::reference()->PushScene(builder.BuildSettingsMenu());
            menu_->set_visible(false);
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
TEXT_MANAGER()->setFont("data/font/Filmcryptic.ttf", 50, NULL);
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
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Continue"));
            break;
        case MenuBuilder::PAUSE_SELECT_EXIT_GAME:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Return to Menu"));
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
    MultiPageMenu *menu = new MultiPageMenu(1, 1);

    // Setting its handler.
    menu->set_handler(new HelpMenuHandler(menu));

    Image* img = TEXT_LOADER()->GetImage("Back");

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

Menu *MenuBuilder::BuildSettingsMenu () {

    // Our main menu.
    Menu *menu = new Menu(MenuBuilder::SETTINGS_SELECT_NUM);
    

    // Setting its handler.
    SettingsMenuHandler *settings_handler_ = new SettingsMenuHandler(menu); 
    menu->set_handler(settings_handler_);

    // The menu's content box.
    menu->set_content_box(Frame(SET_LEFT, SET_TOP, SET_RIGHT, SET_BOTTOM));
    
    // Setting the selection sprite.
    Sprite *selection_sprite = new Sprite;
    selection_sprite->Initialize(VIDEO_MANAGER()->LoadImage("data/images/selection.png"));
    selection_sprite->set_hotspot(Vector2D((SELECTION_WIDTH/2.0f-RECT_WIDTH),( SELECTION_HEIGHT-RECT_HEIGHT)/2.0f));
    menu->set_selection_sprite(selection_sprite);

    // The game logo.
    Sprite *logo = new Sprite;
    logo->Initialize(VIDEO_MANAGER()->LoadImage("data/images/logo_560x334_black.png"));
    logo->set_hotspot(Vector2D(logo->image()->width() * 0.5f, 0));
    menu->AddSprite(logo, Vector2D(VIDEO_MANAGER()->video_size().x/2.0f, 0.0f));
    
    // The sprite of each option.
    settings_handler_->BuildSprites();
    
    return menu;
}

void MenuBuilder::SettingsMenuHandler::Handle(int selection) {
    Settings settings = Settings(std::string("settings.duh"));
   switch (selection) {
        case MenuBuilder::SETTINGS_SELECT_RESOLUTION: {
            resolution_sprites_[sprites_active_[0]]->set_visible(false);
            sprites_active_[0] = (++sprites_active_[0])%NUM_RESOL;
            settings.set_resolution(sprites_active_[0]);
            resolution_sprites_[sprites_active_[0]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_FULLSCREEN: {
            on_off_sprites_[0][sprites_active_[1]]->set_visible(false);
            sprites_active_[1] = (++sprites_active_[1])%NUM_ON_OFF;
            settings.set_fullscreen(sprites_active_[1]);
            on_off_sprites_[0][sprites_active_[1]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_MUSIC: {
            on_off_sprites_[1][sprites_active_[2]]->set_visible(false);
            sprites_active_[2] = (++sprites_active_[2])%NUM_ON_OFF;
            on_off_sprites_[1][sprites_active_[2]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_SOUNDS: {
            on_off_sprites_[2][sprites_active_[3]]->set_visible(false);
            sprites_active_[3] = (++sprites_active_[3])%NUM_ON_OFF;
            on_off_sprites_[2][sprites_active_[3]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_LANGUAGE: {
            language_sprites_[sprites_active_[4]]->set_visible(false);
            sprites_active_[4] = (++sprites_active_[4])%NUM_ON_OFF;
            language_sprites_[sprites_active_[4]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_EXIT: {
            menu_->Finish();
            break;
        }
        default: {
            break;
        }
    }
}

void MenuBuilder::SettingsMenuHandler::BuildSprites() {
    TEXT_MANAGER()->setFont("data/font/Filmcryptic.ttf", 50, NULL);
    
    for (int i = 0; i < MenuBuilder::SETTINGS_SELECT_NUM; ++i) {
        settings_sprites_[i] = new Sprite;
        settings_images_[i] = TEXT_MANAGER()->LoadLine(settings_[i]);
        settings_sprites_[i]->Initialize(settings_images_[i]);
        settings_sprites_[i]->set_hotspot(Vector2D(settings_images_[i]->width() * 0.5f, 0));
        menu_->set_option_sprite(i, settings_sprites_[i]);
    }
    
    for (int i = 0; i < 12; ++i) {
        resolution_sprites_[i] = new Sprite;
        resolution_images_[i] = TEXT_MANAGER()->LoadLine(resolution_[i]);
        resolution_sprites_[i]->Initialize(resolution_images_[i]);
        resolution_sprites_[i]->set_hotspot(Vector2D(resolution_images_[i]->width() * 0.5f, 0));
        menu_->AddSprite(resolution_sprites_[i], framework::Vector2D (SET_RIGHT+10, SET_TOP+14));
        if ( i != 0 ) resolution_sprites_[i]->set_visible(false);
    }
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            on_off_sprites_[i][j] = new Sprite;
            on_off_images_[i][j] = TEXT_MANAGER()->LoadLine(on_off_[j]);
            on_off_sprites_[i][j]->Initialize(on_off_images_[i][j]);
            on_off_sprites_[i][j]->set_hotspot(Vector2D(on_off_images_[i][j]->width() * 0.5f, 0));
            menu_->AddSprite(on_off_sprites_[i][j], framework::Vector2D (SET_RIGHT+10, SET_TOP+(i+1)*75+14));
            if ( j != 0 ) on_off_sprites_[i][j]->set_visible(false);
        }
    }
    
    for (int i = 0; i < 2; ++i) {
        language_sprites_[i] = new Sprite;
        language_images_[i] = TEXT_MANAGER()->LoadLine(language_[i]);
        language_sprites_[i]->Initialize(language_images_[i]);
        language_sprites_[i]->set_hotspot(Vector2D(language_images_[i]->width() * 0.5f, 0));
        menu_->AddSprite(language_sprites_[i], framework::Vector2D (SET_RIGHT+10, SET_TOP+4*75+14));
        if ( i != 0 ) language_sprites_[i]->set_visible(false);
    }
    
}

void MenuBuilder::SettingsMenuHandler::CleanUp() {
    for ( int i = 0; i < 10; ++i ) {
        //settings_images_[i]->Destroy();
        //delete settings_images_[i];
    }
}

}
