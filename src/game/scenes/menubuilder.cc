#include <sstream>
#include "menubuilder.h"
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/animationset.h>
#include <ugdk/util/animationparser.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable/text.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/solidrectangle.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include "game/utils/levelmanager.h"
#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/textloader.h"
#include "game/utils/settings.h"
#include "game/utils/constants.h"
#include "editor/mapeditor.h"
#include "world.h"
#include "menuhandler.h"
#include "menu.h"

namespace scene {

using namespace ugdk;
using namespace utils;
using namespace std;

#define RECT_WIDTH              266
#define RECT_HEIGHT             60
#define SET_RECT_WIDTH          266
#define SET_RECT_HEIGHT         70
#define SELECTION_WIDTH         864
#define SELECTION_HEIGHT        155

#define MENU_TOP                VIDEO_MANAGER()->video_size().y/2.3
#define MENU_BOTTOM             MENU_TOP + MenuBuilder::MAIN_SELECT_NUM*RECT_HEIGHT
#define MENU_LEFT               VIDEO_MANAGER()->video_size().x/2.0 - RECT_WIDTH/2.0
#define MENU_RIGHT              VIDEO_MANAGER()->video_size().x/2.0 + RECT_WIDTH/2.0

#define PAUSE_TOP               VIDEO_MANAGER()->video_size().y/2.0
#define PAUSE_BOTTOM            MENU_TOP + MenuBuilder::PAUSE_SELECT_NUM*RECT_HEIGHT*1.5
#define PAUSE_LEFT              VIDEO_MANAGER()->video_size().x/2.0 - RECT_WIDTH/2.0
#define PAUSE_RIGHT             VIDEO_MANAGER()->video_size().x/2.0 + RECT_WIDTH/2.0

#define SET_TOP                 VIDEO_MANAGER()->video_size().y / 8.0
#define SET_BOTTOM              VIDEO_MANAGER()->video_size().y * (7.0 / 8.0)
#define SET_LEFT                VIDEO_MANAGER()->video_size().x * 0.25
#define SET_RIGHT               VIDEO_MANAGER()->video_size().x * 0.40

#define NUM_RESOL               12
#define NUM_ON_OFF              2
#define SELECTION_SPRITE        2

AnimationSet    *MenuBuilder::ANIMATIONS = NULL;
uint32          MenuBuilder::SELECTION_EYE = -1,
                MenuBuilder::HERO_SHOOTING = -1,
                MenuBuilder::MUMMY_DYING = -1,
                MenuBuilder::MOUSE_CLICKS[2] = { -1, -1 },
                MenuBuilder::FIREBALL = -1,
                MenuBuilder::EARTHQUAKE = -1;

void MenuBuilder::InitializeAnimations() {
    ANIMATIONS = RESOURCE_MANAGER()->animation_loader().Load("animations/menu.gdd");
    SELECTION_EYE = ANIMATIONS->MakeIndex("SELECTION_EYE");
    HERO_SHOOTING = ANIMATIONS->MakeIndex("HERO_SHOOTING");
    MUMMY_DYING = ANIMATIONS->MakeIndex("MUMMY_DYING");
    for (int i = 0; i < 2; i++) {
        string name = "MOUSE_CLICKS_"+string(1, '0'+i);
        MOUSE_CLICKS[i] = ANIMATIONS->MakeIndex(name);
    }
    FIREBALL = ANIMATIONS->MakeIndex("FIREBALL");
    EARTHQUAKE = ANIMATIONS->MakeIndex("EARTHQUAKE");
}

void MenuBuilder::ReleaseAnimations() {
    ANIMATIONS = NULL;
    SELECTION_EYE = -1;
    HERO_SHOOTING = -1;
    MUMMY_DYING = -1;
    for (int i = 0; i < 2; ++i)
        MOUSE_CLICKS[i] = -1;
    FIREBALL = -1;
    EARTHQUAKE = -1;
}

void MenuBuilder::CreateSelectionSprites(Menu* menu, double height) {
    //TODO: put on image factory!!!
    ugdk::graphic::Spritesheet *menu_eye_sheet = RESOURCE_MANAGER()->spritesheet_container().Find("images/eye.png");

    ugdk::graphic::Drawable *selection_sprite[SELECTION_SPRITE];
    for (int i = 0; i < SELECTION_SPRITE; i++) {
        ugdk::graphic::Sprite* sprite = new ugdk::graphic::Sprite(menu_eye_sheet, ANIMATIONS);
        sprite->SelectAnimation(SELECTION_EYE);
        sprite->set_hotspot(ugdk::graphic::Drawable::CENTER);
        selection_sprite[i] = sprite;
        //selection_sprite[i]->set_hotspot(Vector2D(frame_size.x - (1 - i) * frame_size.x, height/2));
    }
    menu->set_selection_sprite(selection_sprite);
}

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
    CreateSelectionSprites(menu,
                           (MENU_BOTTOM-MENU_TOP)/MenuBuilder::MAIN_SELECT_NUM);

    // The game logo.
    ugdk::graphic::Drawable *logo = new ugdk::graphic::TexturedRectangle(RESOURCE_MANAGER()->texture_container().Load("images/logo_560x334_black.png"));
    menu->AddDrawable(logo, Vector2D((VIDEO_MANAGER()->video_size().x - logo->width()) * 0.5, 0.0));

    // The sprite of each option.
    for (int i = 0; i < MenuBuilder::MAIN_SELECT_NUM; ++i) {
        ugdk::graphic::Drawable *options_sprite = NULL;
        switch (i) {
        case MenuBuilder::MAIN_SELECT_PLAY:
            options_sprite = TEXT_LOADER()->GetImage("Play");
            break;
        case MenuBuilder::MAIN_SELECT_EDITOR:
            options_sprite = TEXT_LOADER()->GetImage("Editor");
            break;
        case MenuBuilder::MAIN_SELECT_SETTINGS:
            options_sprite = TEXT_LOADER()->GetImage("Settings");
            break;
        case MenuBuilder::MAIN_SELECT_ABOUT:
            options_sprite = TEXT_LOADER()->GetImage("Credits");
            break;
        case MenuBuilder::MAIN_SELECT_EXIT:
            options_sprite = TEXT_LOADER()->GetImage("Exit");
            break;
        }
        menu->set_option_sprite(i, options_sprite);
    }

    ugdk::graphic::Drawable *version = TEXT_MANAGER()->GetText(Constants::VERSION, L"FontD");
    version->set_hotspot(ugdk::graphic::Drawable::BOTTOM_LEFT);
    menu->AddDrawable(version, Vector2D(10.0, VIDEO_MANAGER()->video_size().y - 10.0));

    ugdk::graphic::Drawable *developed_by = new ugdk::graphic::TexturedRectangle(RESOURCE_MANAGER()->texture_container().Load("images/developed_by_uspgamedev1.png"));
    developed_by->set_hotspot(ugdk::graphic::Drawable::BOTTOM_RIGHT);
    menu->AddDrawable(developed_by, VIDEO_MANAGER()->video_size() + Vector2D(-15.0, 0.0));

    return menu;
}

void MenuBuilder::MainMenuHandler::Handle(int selection, int modifier) {
    if (modifier) return;
    switch (selection) {
        case MenuBuilder::MAIN_SELECT_PLAY: {
            menu_->Hide();
            LevelManager::reference()->ShowIntro();
            break;
        }
        case MenuBuilder::MAIN_SELECT_EDITOR: {
            Engine::reference()->PushScene(new editor::MapEditor());
            menu_->Hide();
            break;
        }
        case MenuBuilder::MAIN_SELECT_SETTINGS: {
            MenuBuilder builder;
            Engine::reference()->PushScene(builder.BuildSettingsMenu());
            menu_->Hide();
            break;
        }
        case MenuBuilder::MAIN_SELECT_ABOUT: {
            menu_->Hide();
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

    // Setting its handler.
    menu->set_handler(new PauseMenuHandler(menu));

    // The menu's content box.
    menu->set_content_box(Frame(PAUSE_LEFT, PAUSE_TOP, PAUSE_RIGHT, PAUSE_BOTTOM));

    // Setting the selection sprite.
    CreateSelectionSprites(menu,
                           (PAUSE_BOTTOM-PAUSE_TOP)/MenuBuilder::PAUSE_SELECT_NUM);

    // The pause bg sprite.
    ugdk::graphic::SolidRectangle* bg = new ugdk::graphic::SolidRectangle(VIDEO_MANAGER()->video_size());
    bg->set_color(ugdk::Color(0.5, 0.5, 0.5, 0.5));
    menu->AddDrawable(bg, Vector2D());

    // The sprite of each option.
    for (int i = 0; i < MenuBuilder::PAUSE_SELECT_NUM; ++i) {
        ugdk::graphic::Drawable *options_sprite = NULL;
        switch (i) {
        case MenuBuilder::PAUSE_SELECT_CONTINUE:
            options_sprite = TEXT_LOADER()->GetImage("Continue");
            break;
        case MenuBuilder::PAUSE_SELECT_EXIT_GAME:
            options_sprite = TEXT_LOADER()->GetImage("Return to Menu");
            break;
        }
        menu->set_option_sprite(i, options_sprite);
    }

    menu->StopsPreviousMusic(false);

    return menu;

}

void MenuBuilder::PauseMenuHandler::Handle(int selection, int modifier) {
    if (modifier) return;
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

void MenuBuilder::PauseMenuHandler::CleanUp() {}

//========================
//   SettingsMenu

string  MenuBuilder::SettingsMenuHandler::settings_names_[MenuBuilder::SETTINGS_SELECT_NUM] = {
     "Resolution", "Fullscreen", "Music", "Sound Effects", "Language", "BLANK", "Apply", "Back"};
string  MenuBuilder::SettingsMenuHandler::on_off_[2] = {"Off", "On" };

MenuBuilder::SettingsMenuHandler::SettingsMenuHandler(Menu *menu) : MenuHandler(menu) {
    for(int i = 0; i < 5; ++i)
        sprites_active_[i] = 0;
    settings_ = Settings::reference();
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
    CreateSelectionSprites(menu,
                           (SET_BOTTOM-SET_TOP)/MenuBuilder::SETTINGS_SELECT_NUM);
    
    // The sprite of each option.
    settings_handler_->BuildSprites();
    
    return menu;
}

void MenuBuilder::SettingsMenuHandler::Handle(int selection, int modifier) {
   if (!modifier) modifier = 1;
   switch (selection) {
        case MenuBuilder::SETTINGS_SELECT_RESOLUTION: {
            resolution_sprites_[sprites_active_[0]]->modifier()->set_visible(false);
            sprites_active_[0] = (sprites_active_[0] + modifier)%NUM_RESOL;
            if (sprites_active_[0] < 0) sprites_active_[0] += NUM_RESOL;
            settings_->set_resolution(sprites_active_[0]);
            resolution_sprites_[sprites_active_[0]]->modifier()->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_FULLSCREEN: {
            on_off_sprites_[0][sprites_active_[1]]->modifier()->set_visible(false);
            sprites_active_[1] = (sprites_active_[1] + modifier)%NUM_ON_OFF;
            if (sprites_active_[1] < 0) sprites_active_[1] += NUM_ON_OFF;
            settings_->set_fullscreen(sprites_active_[1]);
            on_off_sprites_[0][sprites_active_[1]]->modifier()->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_MUSIC: {
            on_off_sprites_[1][sprites_active_[2]]->modifier()->set_visible(false);
            sprites_active_[2] = (sprites_active_[2] + modifier)%NUM_ON_OFF;
            if (sprites_active_[2] < 0) sprites_active_[2] += NUM_ON_OFF;
            settings_->set_background_music(sprites_active_[2]);
            on_off_sprites_[1][sprites_active_[2]]->modifier()->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_SOUNDS: {
            on_off_sprites_[2][sprites_active_[3]]->modifier()->set_visible(false);
            sprites_active_[3] = (sprites_active_[3] + modifier)%NUM_ON_OFF;
            if (sprites_active_[3] < 0) sprites_active_[3] += NUM_ON_OFF;
            settings_->set_sound_effects(sprites_active_[3]);
            on_off_sprites_[2][sprites_active_[3]]->modifier()->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_LANGUAGE: {
            language_sprites_[sprites_active_[4]]->modifier()->set_visible(false);
            sprites_active_[4] = (sprites_active_[4] + modifier)%NUM_ON_OFF;
            if (sprites_active_[4] < 0) sprites_active_[4] += NUM_ON_OFF;
            settings_->set_language(sprites_active_[4]);
            language_sprites_[sprites_active_[4]]->modifier()->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_APPLY: {
            settings_->WriteToDisk();
            utils::LevelManager::reference()->QueueRestartGame();
            ugdk::Engine::reference()->quit();
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
    // Creates the sprites for the setting names
    for (int i = 0; i < MenuBuilder::SETTINGS_SELECT_NUM; ++i) {
        if(settings_names_[i].compare("BLANK") == 0)
            continue;
        ugdk::graphic::Drawable* img = TEXT_LOADER()->GetImage(settings_names_[i]);
        menu_->set_option_sprite(i, img);
    }

    double second_column_x = VIDEO_MANAGER()->video_size().x * 0.75;

    // Get current resolution
    sprites_active_[0] = settings_->resolution();

    const Vector2D *resolutions = settings_->ResolutionList();

    resolution_sprites_ = new ugdk::graphic::Node*[Settings::NUM_RESOLUTIONS];

    //TEXT_LOADER()->SetFont("FontB");
    // Creates the resolution names vector.
    for (int i = 0; i < Settings::NUM_RESOLUTIONS; ++i) {
        std::wostringstream stm;
        stm << static_cast<int>(resolutions[i].x) << L"x" << static_cast<int>(resolutions[i].y);
        ugdk::graphic::Drawable* tex = TEXT_MANAGER()->GetText(stm.str(), L"FontB");
        tex->set_hotspot(ugdk::graphic::Drawable::CENTER);
        resolution_sprites_[i] = new ugdk::graphic::Node(tex);
        resolution_sprites_[i]->modifier()->set_offset(Vector2D(second_column_x, menu_->get_selection_position(0).y));
        menu_->AddNode(resolution_sprites_[i]);
        if ( i != sprites_active_[0] ) resolution_sprites_[i]->modifier()->set_visible(false);
    }
    
    sprites_active_[1] = settings_->fullscreen();
    sprites_active_[2] = settings_->background_music();
    sprites_active_[3] = settings_->sound_effects();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            ugdk::graphic::Drawable *img = TEXT_LOADER()->GetImage(on_off_[j]);
            img->set_hotspot(ugdk::graphic::Drawable::CENTER);
            on_off_sprites_[i][j] = new ugdk::graphic::Node(img);
            on_off_sprites_[i][j]->modifier()->set_offset(Vector2D(second_column_x, menu_->get_selection_position(i+1).y));
            menu_->AddNode(on_off_sprites_[i][j]);
            if ( j != sprites_active_[i+1] ) on_off_sprites_[i][j]->modifier()->set_visible(false);
        }
    }
    
    const string *language_name = settings_->LanguageNameList();
    language_sprites_ = new ugdk::graphic::Node*[Settings::NUM_LANGUAGES];

    sprites_active_[4] = settings_->language();
    for (int i = 0; i < Settings::NUM_LANGUAGES; ++i) {
        ugdk::graphic::Drawable* img = TEXT_LOADER()->GetImage(language_name[i]);
        img->set_hotspot(ugdk::graphic::Drawable::CENTER);
        
        language_sprites_[i] = new ugdk::graphic::Node(img);
        language_sprites_[i]->modifier()->set_offset(Vector2D(second_column_x, menu_->get_selection_position(4).y));
        menu_->AddNode(language_sprites_[i]);
        if ( i != sprites_active_[4] ) language_sprites_[i]->modifier()->set_visible(false);
    }
}

void MenuBuilder::SettingsMenuHandler::CleanUp() {
    delete[] resolution_sprites_;
    delete[] language_sprites_;
}

}
