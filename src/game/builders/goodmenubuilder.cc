#include <functional>

#include <ugdk/action/generictask.h>
#include <ugdk/action/scene.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/text.h>
#include <ugdk/graphic/drawable/solidrectangle.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/ui/menu.h>
#include <ugdk/ui/uielement.h>

#include "goodmenubuilder.h"

#include "game/scenes/world.h"
#include "game/scenes/menubuilder.h"
#include "game/utils/levelmanager.h"
#include "game/utils/menuimagefactory.h"

#include "editor/mapeditor.h"

using std::tr1::bind;
using namespace std::tr1::placeholders;
using ugdk::base::ResourceManager;
using ugdk::graphic::Text;
using ugdk::ui::Menu;
using ugdk::action::Scene;
using ugdk::ui::UIElement;
using ugdk::graphic::Node;

namespace builder {

void Focus_Callback(Scene* scene) {
    scene->interface_node()->set_active(true);
}

void DeFocus_Callback(Scene* scene) {
    scene->interface_node()->set_active(false);
}

void PauseContinueCallback(Scene* menu, const UIElement * source) {
    menu->Finish();
}

void PauseExitCallback(Scene* menu, const UIElement * source) {
    menu->Finish();
    WORLD()->FinishLevel(utils::LevelManager::FINISH_QUIT);
}

void MainMenuPlay(Scene* menu, const UIElement * source) {
    utils::LevelManager::reference()->ShowIntro();
}

void MainMenuEditor(Scene* menu, const UIElement * source) {
    ugdk::Engine::reference()->PushScene(new editor::MapEditor());
}

void MainMenuSettings(Scene* menu, const UIElement * source) {
    scene::MenuBuilder builder;
    //ugdk::Engine::reference()->PushScene(builder.BuildSettingsMenu());
}

void MainMenuCredits(Scene* menu, const UIElement * source) {
    utils::LevelManager::reference()->ShowCredits();
}

void MainMenuExit(Scene* menu, const UIElement * source) {
    ugdk::Engine::reference()->quit();
}


//void FinishMenuCallback(Menu* menu) {
//    menu->FinishScene(menu);
//}

Scene* MenuBuilder::PauseMenu() const {
    ugdk::action::Scene* pause_menu = new Scene();
    ugdk::Vector2D origin(0.0, 0.0), target = VIDEO_MANAGER()->video_size();
    ugdk::ikdtree::Box<2> box(origin.val, target.val);
    utils::MenuImageFactory mif;

    Menu* menu = new Menu(box, Vector2D(0.0, 0.0), ugdk::graphic::Drawable::CENTER);
    menu->SetOptionDrawable(mif.HorusEye(), 0);
    menu->SetOptionDrawable(mif.HorusEye(), 1);

    Text* cont_text = ResourceManager::CreateTextFromLanguageTag("Continue");
    Text* exit_text = ResourceManager::CreateTextFromLanguageTag("Return to Menu");

    ugdk::Vector2D cont_position = target * 0.5;
    cont_position.y -= cont_text->size().y;

    ugdk::Vector2D exit_position = target * 0.5;
    exit_position.y += exit_text->size().y;

    menu->AddObject(new UIElement(cont_position, cont_text, bind(PauseContinueCallback, pause_menu, _1)));
    menu->AddObject(new UIElement(exit_position, exit_text, bind(PauseExitCallback, pause_menu, _1)));

    pause_menu->StopsPreviousMusic(false);
    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    pause_menu->interface_node()->AddChild(menu->node());
    pause_menu->AddEntity(menu);

    ugdk::graphic::SolidRectangle* bg = new ugdk::graphic::SolidRectangle(target);
    bg->set_color(ugdk::Color(0.5, 0.5, 0.5, 0.5));
    pause_menu->interface_node()->set_drawable(bg);

    return pause_menu;
}

Scene* MenuBuilder::MainMenu() const {
    ugdk::action::Scene* main_menu = new Scene();
    main_menu->set_focus_callback(Focus_Callback);
    main_menu->set_defocus_callback(DeFocus_Callback);

    ugdk::Vector2D origin(0.0, 0.0), target = VIDEO_MANAGER()->video_size();
    ugdk::ikdtree::Box<2> box(origin.val, target.val);
    utils::MenuImageFactory mif;

    ugdk::graphic::Drawable *logo = new ugdk::graphic::TexturedRectangle(RESOURCE_MANAGER()->GetTextureFromFile("images/logo_560x334_black.png"));
    logo->set_hotspot(ugdk::graphic::Drawable::TOP);
    Node* logo_node = new Node(logo);
    logo_node->modifier()->set_offset(Vector2D(target.x * 0.5, 0.0));

    ugdk::graphic::Drawable *version = TEXT_MANAGER()->GetText(utils::Constants::VERSION, "FontD");
    version->set_hotspot(ugdk::graphic::Drawable::BOTTOM_LEFT);
    Node* version_node = new Node(version);
    version_node->modifier()->set_offset(Vector2D(10.0, target.y - 10.0));

    ugdk::graphic::Drawable *developed_by = new ugdk::graphic::TexturedRectangle(RESOURCE_MANAGER()->GetTextureFromFile("images/developed_by_uspgamedev1.png"));
    developed_by->set_hotspot(ugdk::graphic::Drawable::BOTTOM_RIGHT);
    Node* developed_by_node = new Node(developed_by);
    developed_by_node->modifier()->set_offset(VIDEO_MANAGER()->video_size() + Vector2D(-15.0, 0.0));

    main_menu->interface_node()->AddChild(logo_node);
    main_menu->interface_node()->AddChild(version_node);
    main_menu->interface_node()->AddChild(developed_by_node);

    Menu* menu = new Menu(box, Vector2D(0.0, 0.0), ugdk::graphic::Drawable::CENTER);
    menu->SetOptionDrawable(mif.HorusEye(), 0);
    menu->SetOptionDrawable(mif.HorusEye(), 1);

    Text* play_text     = ResourceManager::CreateTextFromLanguageTag("Play");
    Text* editor_text   = ResourceManager::CreateTextFromLanguageTag("Editor");
    Text* settings_text = ResourceManager::CreateTextFromLanguageTag("Settings");
    Text* credits_text  = ResourceManager::CreateTextFromLanguageTag("Credits");
    Text* exit_text     = ResourceManager::CreateTextFromLanguageTag("Exit");

    ugdk::Vector2D play_position;
    play_position.x = target.x * 0.5;
    play_position.y = target.y * 0.5 - editor_text->size().y - play_text->size().y + 20.0;

    ugdk::Vector2D editor_position;
    editor_position.x = target.x * 0.5;
    editor_position.y = target.y * 0.5 - editor_text->size().y + 40.0;

    ugdk::Vector2D settings_position;
    settings_position.x = target.x * 0.5;
    settings_position.y = target.y * 0.5 + 60.0;

    ugdk::Vector2D credits_position;
    credits_position.x = target.x * 0.5;
    credits_position.y = target.y * 0.5 + settings_text->size().y + 80.0;

    ugdk::Vector2D exit_position;
    exit_position.x = target.x * 0.5;
    exit_position.y = target.y * 0.5 + settings_text->size().y + credits_text->size().y + 100.0;

    menu->AddObject(new UIElement(play_position,     play_text,     bind(MainMenuPlay, main_menu, _1)));
    menu->AddObject(new UIElement(editor_position,   editor_text,   bind(MainMenuEditor, main_menu, _1)));
    menu->AddObject(new UIElement(settings_position, settings_text, bind(MainMenuSettings, main_menu, _1)));
    menu->AddObject(new UIElement(credits_position,  credits_text,  bind(MainMenuCredits, main_menu, _1)));
    menu->AddObject(new UIElement(exit_position,     exit_text,     bind(MainMenuExit, main_menu, _1)));

    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    main_menu->interface_node()->AddChild(menu->node());
    main_menu->AddEntity(menu);

    return main_menu;
}

}
