#include <sstream>
#include "menubuilder.h"
#include "../../framework/engine.h"
#include "../../framework/textmanager.h"
#include "../../framework/scene.h"
#include "../../framework/animationset.h"
#include "../../framework/animationparser.h"
#include "../../framework/text.h"
#include "../utils/levelmanager.h"
#include "../utils/imagefactory.h"
#include "../utils/hudimagefactory.h"
#include "../utils/textloader.h"
#include "../utils/settings.h"
#include "../utils/constants.h"
#include "../../editor/mapeditor.h"
#include "world.h"
#include "menuhandler.h"
#include "menu.h"
#include "page.h"

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
#define SET_RIGHT               VIDEO_MANAGER()->video_size().x * 0.40f

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
	ANIMATIONS = Engine::reference()->animation_parser()->Load("data/animations/test.and");
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

void MenuBuilder::CreateSelectionSprites(Menu* menu, float height) {
    //TODO: put on image factory!!!
    Image* menu_eye_image = VIDEO_MANAGER()->LoadImage("data/images/eye.png");
    menu_eye_image->set_frame_size(Vector2D(128.0f, 96.0f));

    Sprite *selection_sprite[SELECTION_SPRITE];
    for (int i = 0; i < SELECTION_SPRITE; i++) {
        selection_sprite[i] = new Sprite;
        Vector2D frame_size = menu_eye_image->frame_size();
        selection_sprite[i]->Initialize(menu_eye_image, ANIMATIONS);
        selection_sprite[i]->set_hotspot(Vector2D(frame_size.x - (1 - i) * frame_size.x,
                                                 height/2));
        selection_sprite[i]->SelectAnimation(SELECTION_EYE);
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
    Sprite *logo = new Sprite;
    logo->Initialize(VIDEO_MANAGER()->LoadImage("data/images/logo_560x334_black.png"));
    logo->set_hotspot(Vector2D(logo->size().x * 0.5f, 0));
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
        case MenuBuilder::MAIN_SELECT_EDITOR:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Editor"));
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
        options_sprite->set_hotspot(Vector2D(options_sprite->size().x/2,
                                             /*options_sprite->size().y/2*/0));
        menu->set_option_sprite(i, options_sprite);
    }
	Drawable *img_version = TEXT_MANAGER()->GetText(Constants::VERSION, L"FontD");
    Sprite *version = new Sprite;
    version->Initialize(img_version);
    version->set_hotspot(Vector2D(0, img_version->height()));
    menu->AddSprite(version, Vector2D(0, VIDEO_MANAGER()->video_size().y));

    //Image *img = TEXT_LOADER()->GetImage("DevelopedBy");
    Image *img = VIDEO_MANAGER()->LoadImage("data/images/developed_by_uspgamedev1.png");
    Sprite *developed = new Sprite;
    developed->Initialize(img);
    developed->set_hotspot(Vector2D(0, img->height()));
    menu->AddSprite(developed,
                    Vector2D(VIDEO_MANAGER()->video_size().x - img->width() - 15.0f,
                             VIDEO_MANAGER()->video_size().y));
    //developed->set_zindex(Menu::OPTION_ZINDEX * 0.5f);

    return menu;
}

void MenuBuilder::MainMenuHandler::Handle(int selection, int modifier) {
    if (modifier) return;
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
        case MenuBuilder::MAIN_SELECT_EDITOR: {
            Engine::reference()->PushScene(new editor::MapEditor());
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
	bg_img->set_frame_size(VIDEO_MANAGER()->video_size());
    bg_img->set_color(0.5f, 0.5f, 0.5f);
    bg_img->set_alpha(0.5f);

    // Setting its handler.
    menu->set_handler(new PauseMenuHandler(menu, bg_img));

    // The menu's content box.
    menu->set_content_box(Frame(PAUSE_LEFT, PAUSE_TOP, PAUSE_RIGHT, PAUSE_BOTTOM));

    // Setting the selection sprite.
    CreateSelectionSprites(menu,
                           (PAUSE_BOTTOM-PAUSE_TOP)/MenuBuilder::PAUSE_SELECT_NUM);

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
        options_sprite->set_hotspot(Vector2D(options_sprite->size().x * 0.5f,
                                             /*options_sprite->size().y * 0.5f*/0));
        menu->set_option_sprite(i, options_sprite);
    }

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

void MenuBuilder::PauseMenuHandler::CleanUp() {
    bg_img_->Destroy();
    delete bg_img_;
}

//========================
//   HelpMenu

Menu *MenuBuilder::BuildHelpMenu () {

    // Create Page Manager.
    PageManager *manager = new PageManager(5);

    // Setting its handler.
    manager->set_handler(new PageManagerHandler(manager));

    return manager;
}

// TODO PLEASE REFACTORATE THIS!!!!

void MenuBuilder::CreateBackButton(Page *page) {

    // Setting its handler.
    page->set_handler(new HelpMenuHandler(page));

    Drawable* img = TEXT_LOADER()->GetImage("Back");
    Sprite *options_sprite = new Sprite;
    options_sprite->Initialize(img);
    options_sprite->set_hotspot(Vector2D(options_sprite->size().x/2.0f, 0));

    float top = VIDEO_MANAGER()->video_size().y - img->height();

    // Set page menu
    page->set_content_box(Frame(MENU_LEFT, top, MENU_RIGHT, VIDEO_MANAGER()->video_size().y));
    page->set_option_sprite(0, options_sprite);

    // Setting the selection sprite.
    CreateSelectionSprites(page, img->height());
}

Menu *MenuBuilder::BuildHelpPage1 (PageManager *manager) {

    Page *page = new Page(1, Page::FIRST_PAGE, manager);

    CreateBackButton(page);

    Vector2D spacing = VIDEO_MANAGER()->video_size()*(1.0f/6.0f);

    // The menu content
    Sprite *title = new Sprite;
    title->Initialize(TEXT_LOADER()->GetImage("Help"));
    title->set_hotspot(Vector2D(title->size().x * 0.5f, title->size().y * 0.5f)); 
    page->AddSprite(title, Vector2D(VIDEO_MANAGER()->video_size().x/2.0f, spacing.y*0.5f));
    
    ImageFactory img_fac;

    // A hero animated sprite.
    Sprite *hero_sprite = new Sprite;
    hero_sprite->Initialize(img_fac.HeroImage(), ANIMATIONS);
    hero_sprite->SelectAnimation(HERO_SHOOTING);
    page->AddSprite(hero_sprite, Vector2D(0.0f, spacing.y));

    // A dying mummy sprite.
    Sprite *mummy_sprite = new Sprite;
    mummy_sprite->Initialize(img_fac.MummyImage(), ANIMATIONS);
    mummy_sprite->SelectAnimation(MUMMY_DYING);
    page->AddSprite(mummy_sprite, Vector2D(spacing.x, spacing.y*2.0f));

    // A door sprite.
    Sprite *door_sprite = new Sprite;
    door_sprite->Initialize(img_fac.DoorImage());
    page->AddSprite(door_sprite, Vector2D(spacing.x/2.0f, spacing.y*3.5f));

    Sprite *objective_text[6];
    for (int i = 0; i < 6; ++i)
        objective_text[i] = new Sprite;

    objective_text[0]->Initialize(TEXT_LOADER()->GetImage("Objective"));
    page->AddSprite(objective_text[0], Vector2D(spacing.x*2.0f, spacing.y*1.0f));
    objective_text[1]->Initialize(TEXT_LOADER()->GetImage("HelpObjective1"));
    page->AddSprite(objective_text[1], Vector2D(spacing.x*2.0f, spacing.y*2.0f));
    objective_text[2]->Initialize(TEXT_LOADER()->GetImage("HelpObjective2"));
    page->AddSprite(objective_text[2], Vector2D(spacing.x*2.0f, spacing.y*2.5f));
    objective_text[3]->Initialize(TEXT_LOADER()->GetImage("HelpStairs1"));
    page->AddSprite(objective_text[3], Vector2D(spacing.x*2.0f, spacing.y*3.5f));
    objective_text[4]->Initialize(TEXT_LOADER()->GetImage("HelpStairs2"));
    page->AddSprite(objective_text[4], Vector2D(spacing.x*2.0f, spacing.y*4.0f));
    objective_text[5]->Initialize(TEXT_LOADER()->GetImage("HelpStairs3"));
    page->AddSprite(objective_text[5], Vector2D(spacing.x*2.0f, spacing.y*4.5f));


    return page;
}

Menu *MenuBuilder::BuildHelpPage2 (PageManager *manager) {

    Page *page = new Page(1, Page::NORMAL_PAGE, manager);

    CreateBackButton(page);
    
    // The menu content
    float second_column = VIDEO_MANAGER()->video_size().x/2.0f;
    float spacing = VIDEO_MANAGER()->video_size().y/6.0f;

    //Title
    Sprite *title = new Sprite;
    title->Initialize(TEXT_LOADER()->GetImage("Buttons"));
    title->set_hotspot(Vector2D(title->size().x * 0.5f, title->size().y * 0.5f)); 
    page->AddSprite(title, Vector2D(VIDEO_MANAGER()->video_size().x/2.0f, spacing*0.5f));

    //Keys image
    Image *keys = VIDEO_MANAGER()->LoadImage("data/images/buttons.png");
    long keys_width=keys->width(), keys_height=keys->height()/6.0f;
    keys->set_frame_size(Vector2D(keys_width, keys_height));
    
    //Left column
    Sprite *key_w = new Sprite;
    key_w->Initialize(keys);
    key_w->set_hotspot(Vector2D(0, keys_height*0.5f));
    key_w->SetDefaultFrame(0);
    page->AddSprite(key_w, Vector2D(0, 1.5*spacing));

    Sprite *textkey_w = new Sprite;
    textkey_w->Initialize(TEXT_LOADER()->GetImage("Move Up"));
    textkey_w->set_hotspot(Vector2D(0, textkey_w->size().y*0.5f));
    page->AddSprite(textkey_w, Vector2D(keys_width+10, 1.5*spacing));

    Sprite *key_a = new Sprite;
    key_a->Initialize(keys);
    key_a->set_hotspot(Vector2D(0, keys_height*0.5f));
    key_a->SetDefaultFrame(1);
    page->AddSprite(key_a, Vector2D(0, 2.5*spacing));
    
    Sprite *textkey_a = new Sprite;
    textkey_a->Initialize(TEXT_LOADER()->GetImage("Move Left"));
    textkey_a->set_hotspot(Vector2D(0, textkey_a->size().y*0.5f));
    page->AddSprite(textkey_a, Vector2D(keys_width+10, 2.5*spacing));

    Sprite *key_s = new Sprite;
    key_s->Initialize(keys, ANIMATIONS);
    key_s->set_hotspot(Vector2D(0, keys_height*0.5f));
    key_s->SetDefaultFrame(2);
    page->AddSprite(key_s, Vector2D(0, 3.5*spacing));
    
    Sprite *textkey_s = new Sprite;
    textkey_s->Initialize(TEXT_LOADER()->GetImage("Move Down"));
    textkey_s->set_hotspot(Vector2D(0, textkey_s->size().y*0.5f));
    page->AddSprite(textkey_s, Vector2D(keys_width+10, 3.5*spacing));

    Sprite *key_d = new Sprite;
    key_d->Initialize(keys, ANIMATIONS);
    key_d->set_hotspot(Vector2D(0, keys_height*0.5f));
    key_d->SetDefaultFrame(3);
    page->AddSprite(key_d, Vector2D(0, 4.5*spacing));
    
    Sprite *textkey_d = new Sprite;
    textkey_d->Initialize(TEXT_LOADER()->GetImage("Move Right"));
    textkey_d->set_hotspot(Vector2D(0, textkey_d->size().y*0.5f));
    page->AddSprite(textkey_d, Vector2D(keys_width+10, 4.5*spacing));

    //Right Column
    Sprite *key_q = new Sprite;
    key_q->Initialize(keys, ANIMATIONS);
    key_q->set_hotspot(Vector2D(0, keys_height*0.5f));
    key_q->SetDefaultFrame(4);
    page->AddSprite(key_q, Vector2D(second_column, 1.5*spacing));
    
    Sprite *textkey_q = new Sprite;
    textkey_q->Initialize(TEXT_LOADER()->GetImage("Previous Spell"));
    textkey_q->set_hotspot(Vector2D(0, textkey_q->size().y*0.5f));
    page->AddSprite(textkey_q, Vector2D(second_column+keys_width+10, 1.5*spacing));

    Sprite *key_e = new Sprite;
    key_e->Initialize(keys, ANIMATIONS);
    key_e->set_hotspot(Vector2D(0, keys_height*0.5f));
    key_e->SetDefaultFrame(5);
    page->AddSprite(key_e, Vector2D(second_column, 2.5*spacing));
    
    Sprite *textkey_e = new Sprite;
    textkey_e->Initialize(TEXT_LOADER()->GetImage("Next Spell"));
    textkey_e->set_hotspot(Vector2D(0, textkey_e->size().y*0.5f));
    page->AddSprite(textkey_e, Vector2D(second_column+keys_width+10, 2.5*spacing));

    Image *mouse = VIDEO_MANAGER()->LoadImage("data/images/mouse.png");
    long mouse_width=mouse->width(), mouse_height=mouse->height()/3.0f;
    mouse->set_frame_size(Vector2D(mouse_width, mouse_height));

    Sprite *mouse_left = new Sprite;
    mouse_left->Initialize(mouse, ANIMATIONS);
    mouse_left->set_hotspot(Vector2D(0, mouse_height*0.5f));
    mouse_left->SelectAnimation(MOUSE_CLICKS[0]);
    mouse_left->SetAnimationFPS(2);
    page->AddSprite(mouse_left, Vector2D(second_column, 3.5*spacing));

    Sprite *textmouse_left = new Sprite;
    textmouse_left->Initialize(TEXT_LOADER()->GetImage("Shoot magic"));
    textmouse_left->set_hotspot(Vector2D(0, textmouse_left->size().y*0.5f));
    page->AddSprite(textmouse_left, Vector2D(second_column+mouse_width+10, 3.5*spacing));

    Sprite *mouse_right = new Sprite;
    mouse_right->Initialize(mouse, ANIMATIONS);
    mouse_right->set_hotspot(Vector2D(0, mouse_height*0.5f));
    mouse_right->SelectAnimation(MOUSE_CLICKS[1]);
    mouse_right->SetAnimationFPS(2);
    page->AddSprite(mouse_right, Vector2D(second_column, 4.5*spacing));

    Sprite *textmouse_right = new Sprite;
    textmouse_right->Initialize(TEXT_LOADER()->GetImage("Shoot spell"));
    textmouse_right->set_hotspot(Vector2D(0, textmouse_right->size().y*0.5f));
    page->AddSprite(textmouse_right, Vector2D(second_column+mouse_width+10, 4.5*spacing));

    return page;
}

Menu *MenuBuilder::BuildHelpPage3 (PageManager *manager) {

    Page *page = new Page(1, Page::NORMAL_PAGE, manager);

    CreateBackButton(page);

    float spacing = VIDEO_MANAGER()->video_size().y/5.0f;
    
    // The menu content
    Sprite *title = new Sprite;
    title->Initialize(TEXT_LOADER()->GetImage("Spells"));
    title->set_hotspot(Vector2D(title->size().x * 0.5f, title->size().y * 0.5f)); 
    page->AddSprite(title, Vector2D(VIDEO_MANAGER()->video_size().x/2.0f, spacing*0.5f));
    
    ImageFactory img_fac;

    // A hero animated sprite.
    Sprite *hero_sprite = new Sprite;
    hero_sprite->Initialize(img_fac.HeroImage(), ANIMATIONS);
    hero_sprite->SelectAnimation(HERO_SHOOTING);
    page->AddSprite(hero_sprite, Vector2D(0.0f, 1*spacing));
    
    Sprite *magic_missile = new Sprite;
    magic_missile->Initialize(img_fac.MagicMissileImage());
    page->AddSprite(magic_missile, Vector2D(110.0f, 1.5f*spacing));
    
    Sprite *text_magic0 = new Sprite;
    text_magic0->Initialize(TEXT_LOADER()->GetImage("Help Magic"));
    page->AddSprite(text_magic0, Vector2D(270.0f, spacing));
    
    Sprite *text_magic = new Sprite;
    text_magic->Initialize(TEXT_LOADER()->GetImage("Help StandardSpell"));
    page->AddSprite(text_magic, Vector2D(270.0f, 1.5f*spacing));

    Sprite *fire_ball = new Sprite;
    fire_ball->Initialize(img_fac.ExplosionImage(), ANIMATIONS);
    fire_ball->SelectAnimation(FIREBALL);
    page->AddSprite(fire_ball, Vector2D(0.0f, 2*spacing));
    
    Sprite *text_fire0 = new Sprite;
    text_fire0->Initialize(TEXT_LOADER()->GetImage("Help Fireball0"));
    page->AddSprite(text_fire0, Vector2D(270.0f, 2*spacing));

    Sprite *text_fire1 = new Sprite;
    text_fire1->Initialize(TEXT_LOADER()->GetImage("Help Fireball1"));
    page->AddSprite(text_fire1, Vector2D(270.0f, 2.4*spacing));

    Sprite *text_fire2 = new Sprite;
    text_fire2->Initialize(TEXT_LOADER()->GetImage("Help Fireball2"));
    page->AddSprite(text_fire2, Vector2D(270.0f, 2.7*spacing));

    Sprite *earthquake = new Sprite;
    earthquake->Initialize(img_fac.QuakeImage(), ANIMATIONS);
    earthquake->SelectAnimation(EARTHQUAKE);
    page->AddSprite(earthquake, Vector2D(0.0f, 3*spacing));

    Sprite *text_earthquake0 = new Sprite;
    text_earthquake0->Initialize(TEXT_LOADER()->GetImage("Help Earthquake0"));
    page->AddSprite(text_earthquake0, Vector2D(270.0f, 3*spacing));

    Sprite *text_earthquake1 = new Sprite;
    text_earthquake1->Initialize(TEXT_LOADER()->GetImage("Help Earthquake1"));
    page->AddSprite(text_earthquake1, Vector2D(270.0f, 3.4*spacing));

    Sprite *text_earthquake2 = new Sprite;
    text_earthquake2->Initialize(TEXT_LOADER()->GetImage("Help Earthquake2"));
    page->AddSprite(text_earthquake2, Vector2D(270.0f, 3.7*spacing));

    return page;
}

Menu *MenuBuilder::BuildHelpPage4 (PageManager *manager) {

    Page *page = new Page(1, Page::NORMAL_PAGE, manager);

    CreateBackButton(page);

    float spacing = VIDEO_MANAGER()->video_size().y/5.0f;
    
    // The menu content
    Sprite *title = new Sprite;
    title->Initialize(TEXT_LOADER()->GetImage("Itens"));
    title->set_hotspot(Vector2D(title->size().x * 0.5f, title->size().y * 0.5f)); 
    page->AddSprite(title, Vector2D(VIDEO_MANAGER()->video_size().x/2.0f, spacing*0.5f));

    ImageFactory img_fac;

    Sprite *life = new Sprite;
    life->Initialize(img_fac.LifePotionImage());
    life->set_hotspot(Vector2D(life->size().x * 0.5f, life->size().y * 0.5f));
    page->AddSprite(life, Vector2D(50, 1.5*spacing));
 
    Sprite *text_life1 = new Sprite;
    text_life1->Initialize(TEXT_LOADER()->GetImage("Help Life1"));
    page->AddSprite(text_life1, Vector2D(100, 1*spacing));

    Sprite *text_life2 = new Sprite;
    text_life2->Initialize(TEXT_LOADER()->GetImage("Help Life2"));
    page->AddSprite(text_life2, Vector2D(100.0f, 1.5*spacing));

    Sprite *mana = new Sprite;
    mana->Initialize(img_fac.ManaPotionImage());
    mana->set_hotspot(Vector2D(mana->size().x * 0.5f, mana->size().y * 0.5f));
    page->AddSprite(mana, Vector2D(50, 2.5*spacing));
 
    Sprite *text_mana1 = new Sprite;
    text_mana1->Initialize(TEXT_LOADER()->GetImage("Help Mana1"));
    page->AddSprite(text_mana1, Vector2D(100, 2*spacing));

    Sprite *text_mana2 = new Sprite;
    text_mana2->Initialize(TEXT_LOADER()->GetImage("Help Mana2"));
    page->AddSprite(text_mana2, Vector2D(100.0f, 2.5*spacing));

    Sprite *sight = new Sprite;
    sight->Initialize(img_fac.SightPotionImage());
    sight->set_hotspot(Vector2D(sight->size().x * 0.5f, sight->size().y * 0.5f));
    page->AddSprite(sight, Vector2D(50, 3.5*spacing));
 
    Sprite *text_sight1 = new Sprite;
    text_sight1->Initialize(TEXT_LOADER()->GetImage("Help Sight1"));
    page->AddSprite(text_sight1, Vector2D(100, 3*spacing));

    Sprite *text_sight2 = new Sprite;
    text_sight2->Initialize(TEXT_LOADER()->GetImage("Help Sight2"));
    page->AddSprite(text_sight2, Vector2D(100, 3.5*spacing));

    return page;
}

Menu *MenuBuilder::BuildHelpPage5 (PageManager *manager) {

    Page *page = new Page(1, Page::LAST_PAGE, manager);

    CreateBackButton(page);
    
    Vector2D middle = VIDEO_MANAGER()->video_size();
    middle.x = middle.x*0.5f;
    middle.y = middle.y*0.5f;
    
    // The menu content
    Sprite *hud = new Sprite;
    hud->Initialize(VIDEO_MANAGER()->LoadImage("data/images/hud_help.png"));
    hud->set_hotspot(Vector2D(hud->size().x*0.5f, hud->size().y*0.5f));
    page->AddSprite(hud, Vector2D(VIDEO_MANAGER()->video_size().x*0.5f, VIDEO_MANAGER()->video_size().y*0.5f));

    Sprite *title = new Sprite;
    title->Initialize(TEXT_LOADER()->GetImage("Interface"));
    title->set_hotspot(Vector2D(title->size().x * 0.5f, 0.0f)); 
    page->AddSprite(title, Vector2D(middle.x, 0));

    Sprite *mana = new Sprite;
    mana->Initialize(TEXT_LOADER()->GetImage("Mana Bar"));
    page->AddSprite(mana, Vector2D(middle.x-220, middle.y-215));
    
    Sprite *life = new Sprite;
    life->Initialize(TEXT_LOADER()->GetImage("Life Bar"));
    life->set_hotspot(Vector2D(life->size().x, 0));
    page->AddSprite(life, Vector2D(middle.x+220, middle.y-215));
    
    Sprite *hero = new Sprite;
    hero->Initialize(TEXT_LOADER()->GetImage("Hero"));
    page->AddSprite(hero, Vector2D(middle.x+90, middle.y-50));
    
    Sprite *spell1 = new Sprite;
    spell1->Initialize(TEXT_LOADER()->GetImage("Type of"));
    spell1->set_hotspot(Vector2D(spell1->size().x*0.5f, 0));
    page->AddSprite(spell1, Vector2D(middle.x, middle.y+80));
    
    Sprite *spell2 = new Sprite;
    spell2->Initialize(TEXT_LOADER()->GetImage("Spell"));
    spell2->set_hotspot(Vector2D(spell2->size().x*0.5f, 0));
    page->AddSprite(spell2, Vector2D(middle.x, middle.y+110));
    
    Sprite *counter1 = new Sprite;
    counter1->Initialize(TEXT_LOADER()->GetImage("Missing"));
    counter1->set_hotspot(Vector2D(counter1->size().x*0.5f, 0));
    page->AddSprite(counter1, Vector2D(middle.x+120, middle.y+40));
    
    Sprite *counter2 = new Sprite;
    counter2->Initialize(TEXT_LOADER()->GetImage("Mummies"));
    counter2->set_hotspot(Vector2D(counter2->size().x*0.5f, 0));
    page->AddSprite(counter2, Vector2D(middle.x+120, middle.y+70));
    
    Sprite *counter3 = new Sprite;
    counter3->Initialize(TEXT_LOADER()->GetImage("Counter"));
    counter3->set_hotspot(Vector2D(counter3->size().x*0.5f, 0));
    page->AddSprite(counter3, Vector2D(middle.x+120, middle.y+100));
    
    return page;
}

void MenuBuilder::PageManagerHandler::Handle(int selection, int modifier) {
    if (modifier) return;
    MenuBuilder builder;
    switch(selection){
        case (0): {
            Engine::reference()->PushScene(builder.BuildHelpPage1(static_cast<PageManager*> (menu_)));
            break;
        }
        case (1): {
            Engine::reference()->PushScene(builder.BuildHelpPage2(static_cast<PageManager*> (menu_)));
            break;
        }
        case (2): {
            Engine::reference()->PushScene(builder.BuildHelpPage3(static_cast<PageManager*> (menu_)));
            break;
        }
        case (3): {
            Engine::reference()->PushScene(builder.BuildHelpPage4(static_cast<PageManager*> (menu_)));
            break;
        }
        case (4): {
            Engine::reference()->PushScene(builder.BuildHelpPage5(static_cast<PageManager*> (menu_)));
            break;
        }
        default: {
            break;
        }
    }
}

void MenuBuilder::HelpMenuHandler::Handle(int selection, int modifier) {
    switch(selection){
        case MenuBuilder::HELP_SELECT_BACK: {
            menu_->Finish();
            break;
        }
        case MenuBuilder::HELP_SELECT_LEFT: {
            menu_->Finish();
            break;
        }
        case MenuBuilder::HELP_SELECT_RIGHT: {
            menu_->Finish();
            break;
        }
        default: {
            break;
        }
    }
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
    menu->set_content_box(Frame(SET_LEFT, SET_TOP, SET_RIGHT, SET_BOTTOM), ALIGNMENT_CENTER);

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
            resolution_sprites_[sprites_active_[0]]->set_visible(false);
            sprites_active_[0] = (sprites_active_[0] + modifier)%NUM_RESOL;
            if (sprites_active_[0] < 0) sprites_active_[0] += NUM_RESOL;
            settings_->set_resolution(sprites_active_[0]);
            resolution_sprites_[sprites_active_[0]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_FULLSCREEN: {
            on_off_sprites_[0][sprites_active_[1]]->set_visible(false);
            sprites_active_[1] = (sprites_active_[1] + modifier)%NUM_ON_OFF;
            if (sprites_active_[1] < 0) sprites_active_[1] += NUM_ON_OFF;
            settings_->set_fullscreen(sprites_active_[1]);
            on_off_sprites_[0][sprites_active_[1]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_MUSIC: {
            on_off_sprites_[1][sprites_active_[2]]->set_visible(false);
            sprites_active_[2] = (sprites_active_[2] + modifier)%NUM_ON_OFF;
            if (sprites_active_[2] < 0) sprites_active_[2] += NUM_ON_OFF;
            settings_->set_background_music(sprites_active_[2]);
            on_off_sprites_[1][sprites_active_[2]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_SOUNDS: {
            on_off_sprites_[2][sprites_active_[3]]->set_visible(false);
            sprites_active_[3] = (sprites_active_[3] + modifier)%NUM_ON_OFF;
            if (sprites_active_[3] < 0) sprites_active_[3] += NUM_ON_OFF;
            settings_->set_sound_effects(sprites_active_[3]);
            on_off_sprites_[2][sprites_active_[3]]->set_visible(true);
            break;
        }
        case MenuBuilder::SETTINGS_SELECT_LANGUAGE: {
            language_sprites_[sprites_active_[4]]->set_visible(false);
            sprites_active_[4] = (sprites_active_[4] + modifier)%NUM_ON_OFF;
            if (sprites_active_[4] < 0) sprites_active_[4] += NUM_ON_OFF;
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
    Sprite *options[MenuBuilder::SETTINGS_SELECT_NUM];

    // Creates the sprites for the setting names
    for (int i = 0; i < MenuBuilder::SETTINGS_SELECT_NUM; ++i) {
        if(settings_names_[i].compare("BLANK") == 0)
            continue;
        Drawable* img = TEXT_LOADER()->GetImage(settings_names_[i]);
        options[i] = new Sprite;
        options[i]->Initialize(img);
        options[i]->set_hotspot(Vector2D(img->width() / 2, /*img->height() / 2*/0));
        menu_->set_option_sprite(i, options[i]);
    }

    float second_column_x = VIDEO_MANAGER()->video_size().x * 0.75f;

    // Get current resolution
    sprites_active_[0] = settings_->resolution();

    const Vector2D *resolutions = settings_->ResolutionList();

    resolution_sprites_ = static_cast<Sprite**>(malloc(Settings::NUM_RESOLUTIONS * sizeof(Sprite*)));
    resolution_images_ = static_cast<Drawable**>(malloc(Settings::NUM_RESOLUTIONS * sizeof(Drawable*)));

    //TEXT_LOADER()->SetFont("FontB");
    // Creates the resolution names vector.
    for (int i = 0; i < Settings::NUM_RESOLUTIONS; ++i) {
        resolution_sprites_[i] = new Sprite;
        std::wostringstream stm;
        stm << static_cast<int>(resolutions[i].x) << L"x" << static_cast<int>(resolutions[i].y);
        resolution_images_[i] = TEXT_MANAGER()->GetText(stm.str(), L"FontB");
        resolution_sprites_[i]->Initialize(resolution_images_[i]);
        resolution_sprites_[i]->set_hotspot(Vector2D(resolution_images_[i]->width() * 0.5f, /*resolution_images_[i]->height() * 0.5f*/0));
        menu_->AddSprite(resolution_sprites_[i], framework::Vector2D (second_column_x, options[0]->position().y));
        if ( i != sprites_active_[0] ) resolution_sprites_[i]->set_visible(false);
    }
    
    sprites_active_[1] = settings_->fullscreen();
    sprites_active_[2] = settings_->background_music();
    sprites_active_[3] = settings_->sound_effects();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            on_off_sprites_[i][j] = new Sprite;
            Drawable *img = TEXT_LOADER()->GetImage(on_off_[j]);
            on_off_sprites_[i][j]->Initialize(img);
            on_off_sprites_[i][j]->set_hotspot(Vector2D(img->width() * 0.5f, /*img->height() * 0.5f*/0));
            menu_->AddSprite(on_off_sprites_[i][j], framework::Vector2D (second_column_x, options[i+1]->position().y));
            if ( j != sprites_active_[i+1] ) on_off_sprites_[i][j]->set_visible(false);
        }
    }
    
    const string *language_name = settings_->LanguageNameList();
    language_sprites_ = static_cast<Sprite**>(malloc(Settings::NUM_LANGUAGES * sizeof(Sprite*)));

    sprites_active_[4] = settings_->language();
    for (int i = 0; i < Settings::NUM_LANGUAGES; ++i) {
        language_sprites_[i] = new Sprite;
        Drawable* img = TEXT_LOADER()->GetImage(language_name[i]);
        language_sprites_[i]->Initialize(img);
        language_sprites_[i]->set_hotspot(Vector2D(img->width() * 0.5f, /*img->height() * 0.5f*/0));
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
}

}
