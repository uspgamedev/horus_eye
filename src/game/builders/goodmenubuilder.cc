#include <sstream>
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
#include "game/utils/settings.h"

#include "editor/mapeditor.h"

using std::tr1::bind;
using namespace std::tr1::placeholders;
using ugdk::base::ResourceManager;
using ugdk::graphic::Text;
using ugdk::ui::Menu;
using ugdk::action::Scene;
using ugdk::ui::UIElement;
using ugdk::graphic::Node;
using utils::Settings;

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
    MenuBuilder builder;
    ugdk::Engine::reference()->PushScene(builder.SettingsMenu());
}

void MainMenuCredits(Scene* menu, const UIElement * source) {
    utils::LevelManager::reference()->ShowCredits();
}

void SceneExit(Scene* scene, const UIElement * source) {
    scene->Finish();
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
    menu->AddObject(new UIElement(exit_position,     exit_text,     bind(SceneExit, main_menu, _1)));

    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    main_menu->interface_node()->AddChild(menu->node());
    main_menu->AddEntity(menu);

    return main_menu;
}

// Future nuke
static std::string on_off_[2] = {"Off", "On" };


struct SettingsFunction {
    std::string name;
    std::tr1::function<void (utils::Settings*, int)> function;
    int max_val;
};
static SettingsFunction SETTING_FUNCTIONS[] = { 
    { "Resolution",     &Settings::set_resolution, 12 },
    { "Fullscreen",     &Settings::set_fullscreen, 2 },
    { "Music",          &Settings::set_background_music, 2 },
    { "Sound Effects",  &Settings::set_sound_effects, 2 },
    { "Language",       &Settings::set_language, 2 },
};

struct ConveninentSettingsData {
    ugdk::graphic::Node **nodes_[5];
    int sprites_active_[5];
    const SettingsFunction* setting_functions_;
    std::map<const UIElement*, int> indices_;

    ~ConveninentSettingsData() {
        puts("puts puts");
    }
};

static void ChangeSetting(std::tr1::shared_ptr<ConveninentSettingsData> data, int modifier, const UIElement * source) {
    if(data->indices_.find(source) == data->indices_.end()) return;
    int value = data->indices_[source];
    int max_val = data->setting_functions_[value].max_val;
    std::tr1::function<void (utils::Settings*, int)> settingsfunc = data->setting_functions_[value].function;

    data->nodes_[value][data->sprites_active_[value]]->modifier()->set_visible(false);
    data->sprites_active_[value] = (data->sprites_active_[value] + modifier) % max_val;
    if (data->sprites_active_[value] < 0) data->sprites_active_[value] += max_val;

    settingsfunc(Settings::reference(), data->sprites_active_[value]);

    data->nodes_[value][data->sprites_active_[value]]->modifier()->set_visible(true);
}

static void ElementPress(std::tr1::shared_ptr<ConveninentSettingsData> data, const UIElement * source) {
    ChangeSetting(data, +1, source);
}

static void PressArrow(std::tr1::shared_ptr<ConveninentSettingsData> data, int modifier, Menu* menu) {
    ChangeSetting(data, modifier, menu->focused_element());
}

static void ApplySettings(const UIElement * source) {
    Settings::reference()->WriteToDisk();
    utils::LevelManager::reference()->QueueRestartGame();
    ugdk::Engine::reference()->quit();
}

std::tr1::shared_ptr<ConveninentSettingsData> makeSettingsData(Node* node) {
    ConveninentSettingsData* data = new ConveninentSettingsData;
    data->setting_functions_ = SETTING_FUNCTIONS;

    Settings* settings_ = Settings::reference();

    data->sprites_active_[0] = Settings::reference()->resolution();
    data->sprites_active_[1] = settings_->fullscreen();
    data->sprites_active_[2] = settings_->background_music();
    data->sprites_active_[3] = settings_->sound_effects();
    data->sprites_active_[4] = settings_->language();

    const Vector2D *resolutions = Settings::reference()->ResolutionList();
    const std::string *language_name = settings_->LanguageNameList();

    data->nodes_[0] = new ugdk::graphic::Node*[Settings::NUM_RESOLUTIONS];

    double second_column_x = VIDEO_MANAGER()->video_size().x * 0.8;

    // Creates the resolution names vector.
    for (int i = 0; i < Settings::NUM_RESOLUTIONS; ++i) {
        std::wostringstream stm;
        stm << static_cast<int>(resolutions[i].x) << L"x" << static_cast<int>(resolutions[i].y);
        ugdk::graphic::Drawable* tex = TEXT_MANAGER()->GetText(stm.str(), "FontB");
        tex->set_hotspot(ugdk::graphic::Drawable::CENTER);
        data->nodes_[0][i] = new ugdk::graphic::Node(tex);
        data->nodes_[0][i]->modifier()->set_offset(Vector2D(second_column_x, 70.0));
        node->AddChild(data->nodes_[0][i]);
        if ( i != data->sprites_active_[0] ) data->nodes_[0][i]->modifier()->set_visible(false);
    }

    for (int i = 0; i < 3; ++i) {
        data->nodes_[i + 1] = new ugdk::graphic::Node*[2];
        for (int j = 0; j < 2; ++j) {
            ugdk::graphic::Drawable *img = ugdk::base::ResourceManager::CreateTextFromLanguageTag(on_off_[j]);
            img->set_hotspot(ugdk::graphic::Drawable::CENTER);
            data->nodes_[i + 1][j] = new ugdk::graphic::Node(img);

            data->nodes_[i + 1][j]->modifier()->set_offset(Vector2D(second_column_x, 70.0 * (i + 2) ));
            node->AddChild(data->nodes_[i + 1][j]);
            if ( j != data->sprites_active_[i+1] ) data->nodes_[i + 1][j]->modifier()->set_visible(false);
        }
    }
    
    data->nodes_[4] = new ugdk::graphic::Node*[Settings::NUM_LANGUAGES];

    for (int i = 0; i < Settings::NUM_LANGUAGES; ++i) {
        ugdk::graphic::Drawable* img = ugdk::base::ResourceManager::CreateTextFromLanguageTag(language_name[i]);
        img->set_hotspot(ugdk::graphic::Drawable::CENTER);
        
        data->nodes_[4][i] = new ugdk::graphic::Node(img);
        data->nodes_[4][i]->modifier()->set_offset(Vector2D(second_column_x, 70.0 * 5));
        node->AddChild(data->nodes_[4][i]);
        if ( i != data->sprites_active_[4] ) data->nodes_[4][i]->modifier()->set_visible(false);
    }

    return std::tr1::shared_ptr<ConveninentSettingsData>(data);
}

Scene* MenuBuilder::SettingsMenu() const {
    ugdk::action::Scene* settings_menu = new Scene();
    ugdk::Vector2D origin(0.0, 0.0), target = VIDEO_MANAGER()->video_size();
    ugdk::ikdtree::Box<2> box(origin.val, target.val);
    utils::MenuImageFactory mif;

    Menu* menu = new Menu(box, Vector2D(0.0, 0.0), ugdk::graphic::Drawable::LEFT);
    menu->SetOptionDrawable(mif.HorusEye(), 0);
    menu->SetOptionDrawable(mif.HorusEye(), 1);

    std::tr1::shared_ptr<ConveninentSettingsData> data = makeSettingsData(settings_menu->interface_node());
    double left_column = target.x * 0.15;

    for (int i = 0; i < 5; ++i) {
        ugdk::graphic::Drawable* img = ugdk::base::ResourceManager::CreateTextFromLanguageTag(SETTING_FUNCTIONS[i].name);
        ugdk::Vector2D pos = ugdk::Vector2D(left_column, 70.0 * (i + 1));
        UIElement* uie = new UIElement(pos, img, bind(ElementPress, data, _1));
        data->indices_[uie] = i;
        menu->AddObject(uie);
    }

    {   ugdk::graphic::Drawable* img = ugdk::base::ResourceManager::CreateTextFromLanguageTag("Apply");
        ugdk::Vector2D pos = ugdk::Vector2D(left_column, 70.0 * 7);
        menu->AddObject(new UIElement(pos, img, ApplySettings)); }

    {   ugdk::graphic::Drawable* img = ugdk::base::ResourceManager::CreateTextFromLanguageTag("Exit");
        ugdk::Vector2D pos = ugdk::Vector2D(left_column, 70.0 * 8);
        menu->AddObject(new UIElement(pos, img, bind(SceneExit, settings_menu, _1))); }

    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    menu->AddCallback(ugdk::input::K_RIGHT , bind(PressArrow, data, +1, _1));
    menu->AddCallback(ugdk::input::K_LEFT  , bind(PressArrow, data, -1, _1));

    settings_menu->interface_node()->AddChild(menu->node());
    settings_menu->AddEntity(menu);
    return settings_menu;
}

}
