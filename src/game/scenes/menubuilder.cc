#include <sstream>
#include "menubuilder.h"
#include "../../framework/engine.h"
#include "../../framework/textmanager.h"
#include "../utils/levelmanager.h"
#include "../utils/textloader.h"
#include "../utils/settings.h"
#include "../utils/constants.h"
#include "world.h"
#include "menuhandler.h"
#include "menu.h"
#include "multipagemenu.h"

namespace scene {

using namespace framework;
using namespace utils;
using namespace std;

#define RECT_WIDTH              266
#define RECT_HEIGHT             60
#define SET_RECT_WIDTH          266
#define SET_RECT_HEIGHT         70
#define SELECTION_WIDTH         864
#define SELECTION_HEIGHT        155

#define MENU_TOP                VIDEO_MANAGER()->video_size().y/2.3f
#define MENU_BOTTOM             MENU_TOP + MenuBuilder::MAIN_SELECT_NUM*RECT_HEIGHT
#define MENU_LEFT               VIDEO_MANAGER()->video_size().x/2.0f - RECT_WIDTH/2.0f
#define MENU_RIGHT              VIDEO_MANAGER()->video_size().x/2.0f + RECT_WIDTH/2.0f

#define PAUSE_TOP               VIDEO_MANAGER()->video_size().y/2.0f
#define PAUSE_BOTTOM            MENU_TOP + MenuBuilder::PAUSE_SELECT_NUM*RECT_HEIGHT*1.5f
#define PAUSE_LEFT              VIDEO_MANAGER()->video_size().x/2.0f - RECT_WIDTH/2.0f
#define PAUSE_RIGHT             VIDEO_MANAGER()->video_size().x/2.0f + RECT_WIDTH/2.0f

#define SET_TOP                 VIDEO_MANAGER()->video_size().y / 8.0f
#define SET_BOTTOM              VIDEO_MANAGER()->video_size().y * (7.0f / 8.0f)
#define SET_LEFT                VIDEO_MANAGER()->video_size().x * 0.25f
#define SET_RIGHT               VIDEO_MANAGER()->video_size().x * 0.50f

#define NUM_RESOL               12
#define NUM_ON_OFF              2

//========================
//   MainMenu

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
        options_sprite->set_hotspot(Vector2D(options_sprite->image()->width() * 0.5f, 0.0f));
        menu->set_option_sprite(i, options_sprite);
    }

    Image *img = TEXT_LOADER()->GetImage("DevelopedBy");
    Sprite *developed = new Sprite;
    developed->Initialize(img);
    developed->set_hotspot(Vector2D(0, img->height()));
    menu->AddSprite(developed, Vector2D(15.0f, VIDEO_MANAGER()->video_size().y));
    developed->set_zindex(Menu::OPTION_ZINDEX * 0.5f);

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

//========================
//   PauseMenu

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
    for (int i = 0; i < MenuBuilder::PAUSE_SELECT_NUM; ++i) {
        Sprite *options_sprite = new Sprite;
        switch (i) {
        case MenuBuilder::PAUSE_SELECT_CONTINUE:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Continue"));
            break;
        case MenuBuilder::PAUSE_SELECT_EXIT_GAME:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Return to Menu"));
            break;
        }
        options_sprite->set_hotspot(Vector2D(options_sprite->image()->width() * 0.5f, 0.0f));
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

//========================
//   HelpMenu

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
    options_sprite->set_hotspot(Vector2D(options_sprite->image()->width() * 0.5f, 0.0f));
    menu->set_option_sprite(0, options_sprite);

    return menu;
}

void MenuBuilder::HelpMenuHandler::Handle(int selection) {
    menu_->Finish();
}

//========================
//   SettingsMenu

string  MenuBuilder::SettingsMenuHandler::settings_names_[MenuBuilder::SETTINGS_SELECT_NUM] = {
     "Resolution", "Fullscreen", "Music", "Sound Effects", "Language", "BLANK", "Apply", "Back"};
string  MenuBuilder::SettingsMenuHandler::on_off_[2] = {"Off", "On" };

MenuBuilder::SettingsMenuHandler::SettingsMenuHandler(Menu *menu) : MenuHandler(menu) {
    for(int i = 0; i < 5; ++i)
        sprites_active_[i] = 0;
    settings_ = new Settings();
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
    float hotspot_x = selection_sprite->image()->width() * 0.45f;
    selection_sprite->set_hotspot(Vector2D(hotspot_x,( SELECTION_HEIGHT-RECT_HEIGHT)/2.0f));
    menu->set_selection_sprite(selection_sprite);
    
    // The sprite of each option.
    settings_handler_->BuildSprites();
    
    return menu;
}

void MenuBuilder::SettingsMenuHandler::Handle(int selection) {
   switch (selection) {
        case MenuBuilder::SETTINGS_SELECT_RESOLUTION: {
            resolution_sprites_[sprites_active_[0]]->set_visible(false);
            sprites_active_[0] = (sprites_active_[0] + 1)%NUM_RESOL;
            settings_->set_resolution(sprites_active_[0]);
            resolution_sprites_[sprites_active_[0]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_FULLSCREEN: {
            on_off_sprites_[0][sprites_active_[1]]->set_visible(false);
            sprites_active_[1] = (sprites_active_[1] + 1)%NUM_ON_OFF;
            settings_->set_fullscreen(sprites_active_[1]);
            on_off_sprites_[0][sprites_active_[1]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_MUSIC: {
            on_off_sprites_[1][sprites_active_[2]]->set_visible(false);
            sprites_active_[2] = (sprites_active_[2] + 1)%NUM_ON_OFF;
            settings_->set_background_music(sprites_active_[2]);
            on_off_sprites_[1][sprites_active_[2]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_SOUNDS: {
            on_off_sprites_[2][sprites_active_[3]]->set_visible(false);
            sprites_active_[3] = (sprites_active_[3] + 1)%NUM_ON_OFF;
            settings_->set_sound_effects(sprites_active_[3]);
            on_off_sprites_[2][sprites_active_[3]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_LANGUAGE: {
            language_sprites_[sprites_active_[4]]->set_visible(false);
            sprites_active_[4] = (sprites_active_[4] + 1)%NUM_ON_OFF;
            settings_->set_language(sprites_active_[4]);
            language_sprites_[sprites_active_[4]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_APPLY: {
            settings_->WriteToDisk();
            utils::LevelManager::reference()->QueueRestartGame();
            framework::Engine::reference()->quit();
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
    TEXT_MANAGER()->setFont("data/font/Filmcrypob.ttf", 50, NULL);
    
    Sprite *options[MenuBuilder::SETTINGS_SELECT_NUM];

    // Creates the sprites for the setting names
    for (int i = 0; i < MenuBuilder::SETTINGS_SELECT_NUM; ++i) {
        if(settings_names_[i].compare("BLANK") == 0)
            continue;
        Image* img = TEXT_LOADER()->GetImage(settings_names_[i]);
        options[i] = new Sprite;
        options[i]->Initialize(img);
        options[i]->set_hotspot(Vector2D(img->width(), 0.0f));
        menu_->set_option_sprite(i, options[i]);
    }

    float second_column_x = VIDEO_MANAGER()->video_size().x * 0.75f;

    // Get current resolution
    sprites_active_[0] = settings_->resolution();

    const Vector2D *resolutions = settings_->ResolutionList();

    resolution_sprites_ = static_cast<Sprite**>(malloc(Settings::NUM_RESOLUTIONS * sizeof(Sprite*)));
    resolution_images_ = static_cast<Image**>(malloc(Settings::NUM_RESOLUTIONS * sizeof(Image*)));

    // Creates the resolution names vector.
    for (int i = 0; i < Settings::NUM_RESOLUTIONS; ++i) {
        resolution_sprites_[i] = new Sprite;
        std::ostringstream stm;
        stm << static_cast<int>(resolutions[i].x) << "x" << static_cast<int>(resolutions[i].y);
        resolution_images_[i] = TEXT_MANAGER()->LoadFancyLine(stm.str());
        resolution_sprites_[i]->Initialize(resolution_images_[i]);
        resolution_sprites_[i]->set_hotspot(Vector2D(resolution_images_[i]->width() * 0.5f, 0));
        menu_->AddSprite(resolution_sprites_[i], framework::Vector2D (second_column_x, options[0]->position().y));
        if ( i != sprites_active_[0] ) resolution_sprites_[i]->set_visible(false);
    }
    
    sprites_active_[1] = settings_->fullscreen();
    sprites_active_[2] = settings_->background_music();
    sprites_active_[3] = settings_->sound_effects();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            on_off_sprites_[i][j] = new Sprite;
            Image *img = TEXT_LOADER()->GetImage(on_off_[j]);
            on_off_sprites_[i][j]->Initialize(img);
            on_off_sprites_[i][j]->set_hotspot(Vector2D(img->width() * 0.5f, 0));
            menu_->AddSprite(on_off_sprites_[i][j], framework::Vector2D (second_column_x, options[i+1]->position().y));
            if ( j != sprites_active_[i+1] ) on_off_sprites_[i][j]->set_visible(false);
        }
    }
    
    const string *language_name = settings_->LanguageNameList();
    language_sprites_ = static_cast<Sprite**>(malloc(Settings::NUM_LANGUAGES * sizeof(Sprite*)));
    language_images_ = static_cast<Image**>(malloc(Settings::NUM_LANGUAGES * sizeof(Image*)));

    sprites_active_[4] = settings_->language();
    for (int i = 0; i < Settings::NUM_LANGUAGES; ++i) {
        language_sprites_[i] = new Sprite;
        language_images_[i] = TEXT_MANAGER()->LoadFancyLine(language_name[i]);
        language_sprites_[i]->Initialize(language_images_[i]);
        language_sprites_[i]->set_hotspot(Vector2D(language_images_[i]->width() * 0.5f, 0));
        menu_->AddSprite(language_sprites_[i], framework::Vector2D (second_column_x, options[4]->position().y));
        if ( i != sprites_active_[4] ) language_sprites_[i]->set_visible(false);
    }
    
}

void MenuBuilder::SettingsMenuHandler::CleanUp() {
    delete settings_;

    free(resolution_sprites_);
    for(int i = 0; i < Settings::NUM_RESOLUTIONS; ++i) {
        resolution_images_[i]->Destroy();
        delete resolution_images_[i];
    }
    free(resolution_images_);

    free(language_sprites_);
    for(int i = 0; i < Settings::NUM_LANGUAGES; ++i) {
        language_images_[i]->Destroy();
        delete language_images_[i];
    }
    free(language_images_);
}

}
