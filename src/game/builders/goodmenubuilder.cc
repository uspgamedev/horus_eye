#include <sstream>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include FROM_TR1(memory)

#include <ugdk/action/generictask.h>
#include <ugdk/action/scene.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/label.h>
#include <ugdk/ui/menu.h>
#include <ugdk/ui/button.h>
#include <ugdk/util/intervalkdtree.h>

#include "goodmenubuilder.h"

#include "game/constants.h"
#include "game/scenes/world.h"
#include "game/utils/levelmanager.h"
#include "game/utils/menuimagefactory.h"
#include "game/utils/settings.h"

using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
using ugdk::action::Scene;
using ugdk::base::ResourceManager;
using ugdk::graphic::Drawable;
using ugdk::graphic::Node;
using ugdk::ui::Menu;
using ugdk::ui::Button;
using ugdk::ui::UIElement;
using utils::Settings;

static std::wstring convertFromString(const std::string& str) {
    std::wstring str2(str.length(), L' '); // Make room for characters
    std::copy(str.begin(), str.end(), str2.begin());
    return str2;
}

namespace builder {

void Focus_Callback(Scene* scene) {
    scene->interface_node()->set_active(true);
}

void DeFocus_Callback(Scene* scene) {
    scene->interface_node()->set_active(false);
}

void PauseContinueCallback(Scene* menu, const Button * source) {
    menu->Finish();
}

void PauseExitCallback(Scene* menu, const Button * source) {
    menu->Finish();
    WORLD()->FinishLevel(utils::LevelManager::FINISH_QUIT);
}

void MainMenuPlay(Scene* menu, const Button * source) {
    utils::LevelManager::reference()->ShowIntro();
}

void MainMenuSettings(Scene* menu, const Button * source) {
    MenuBuilder builder;
    ugdk::Engine::reference()->PushScene(builder.SettingsMenu());
}

void MainMenuCredits(Scene* menu, const Button * source) {
    utils::LevelManager::reference()->ShowCredits();
}

#ifdef DEBUG
void MainMenuDebugPlay(Scene* menu, const Button * source) {
    utils::LevelManager::reference()->DebugLoadSpecificLevel("debug_level");
}
#endif

void SceneExit(Scene* scene, const Button * source) {
    scene->Finish();
}

Scene* MenuBuilder::PauseMenu() const {
    ugdk::action::Scene* pause_menu = new Scene();
    ugdk::math::Vector2D origin(0.0, 0.0), target = VIDEO_MANAGER()->video_size();
    ugdk::ikdtree::Box<2> box(origin.val, target.val);
    utils::MenuImageFactory mif;

    Menu* menu = new Menu(box, Vector2D(0.0, 0.0), ugdk::graphic::Drawable::CENTER);
    for(int i = 0; i < 2; ++i) {
        ugdk::graphic::Sprite* sprite = mif.HorusEye();
        pause_menu->media_manager().AddPlayer(&sprite->animation_player());
        menu->SetOptionDrawable(sprite, i);
    }

    Drawable* cont_text = ResourceManager::GetLanguageWord("Continue")->CreateLabel();
    Drawable* exit_text = ResourceManager::GetLanguageWord("Return to Menu")->CreateLabel();

    ugdk::math::Vector2D cont_position = target * 0.5;
    cont_position.y -= cont_text->size().y;

    ugdk::math::Vector2D exit_position = target * 0.5;
    exit_position.y += exit_text->size().y;

    menu->AddObject(new Button(cont_position, cont_text, bind(PauseContinueCallback, pause_menu, _1)));
    menu->AddObject(new Button(exit_position, exit_text, bind(PauseExitCallback, pause_menu, _1)));

    pause_menu->StopsPreviousMusic(false);
    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    pause_menu->interface_node()->AddChild(menu->node());
    pause_menu->AddEntity(menu);

    //TODO: solid rectangle no longer exists
    //ugdk::graphic::SolidRectangle* bg = new ugdk::graphic::SolidRectangle(target);
    //bg->set_color(ugdk::Color(0.5, 0.5, 0.5, 0.5));
    //pause_menu->interface_node()->set_drawable(bg);

    return pause_menu;
}

Scene* MenuBuilder::MainMenu() const {
    ugdk::action::Scene* main_menu = new Scene();
    main_menu->set_focus_callback(Focus_Callback);
    main_menu->set_defocus_callback(DeFocus_Callback);

    ugdk::math::Vector2D origin(0.0, 0.0), target = VIDEO_MANAGER()->video_size();
    ugdk::ikdtree::Box<2> box(origin.val, target.val);
    utils::MenuImageFactory mif;

    ugdk::graphic::Drawable *logo = new ugdk::graphic::TexturedRectangle(RESOURCE_MANAGER()->GetTextureFromFile("images/logo_560x334_black.png"));
    logo->set_hotspot(ugdk::graphic::Drawable::TOP);
    Node* logo_node = new Node(logo);
    logo_node->geometry().set_offset(Vector2D(target.x * 0.5, 0.0));

    ugdk::graphic::Drawable *version = new ugdk::graphic::Label(constants::version(), TEXT_MANAGER()->GetFont("FontD"));
    version->set_hotspot(ugdk::graphic::Drawable::BOTTOM_LEFT);
    Node* version_node = new Node(version);
    version_node->geometry().set_offset(Vector2D(10.0, target.y - 10.0));

    ugdk::graphic::Drawable *developed_by = new ugdk::graphic::TexturedRectangle(RESOURCE_MANAGER()->GetTextureFromFile("images/developed_by_uspgamedev1.png"));
    developed_by->set_hotspot(ugdk::graphic::Drawable::BOTTOM_RIGHT);
    Node* developed_by_node = new Node(developed_by);
    developed_by_node->geometry().set_offset(VIDEO_MANAGER()->video_size() + Vector2D(-15.0, 0.0));

    main_menu->interface_node()->AddChild(logo_node);
    main_menu->interface_node()->AddChild(version_node);
    main_menu->interface_node()->AddChild(developed_by_node);

    Menu* menu = new Menu(box, Vector2D(0.0, 0.0), ugdk::graphic::Drawable::CENTER);
    for(int i = 0; i < 2; ++i) {
        ugdk::graphic::Sprite* sprite = mif.HorusEye();
        main_menu->media_manager().AddPlayer(&sprite->animation_player());
        menu->SetOptionDrawable(sprite, i);
    }

    Drawable* play_text     = ResourceManager::GetLanguageWord("Play")->CreateLabel();
    Drawable* settings_text = ResourceManager::GetLanguageWord("Settings")->CreateLabel();
    Drawable* credits_text  = ResourceManager::GetLanguageWord("Credits")->CreateLabel();
    Drawable* exit_text     = ResourceManager::GetLanguageWord("Exit")->CreateLabel();

    ugdk::math::Vector2D play_position;
    play_position.x = target.x * 0.5;
    play_position.y = target.y * 0.5 - play_text->size().y + 20.0;

    ugdk::math::Vector2D settings_position;
    settings_position.x = target.x * 0.5;
    settings_position.y = target.y * 0.5 + 60.0;

    ugdk::math::Vector2D credits_position;
    credits_position.x = target.x * 0.5;
    credits_position.y = target.y * 0.5 + settings_text->size().y + 80.0;

    ugdk::math::Vector2D exit_position;
    exit_position.x = target.x * 0.5;
    exit_position.y = target.y * 0.5 + settings_text->size().y + credits_text->size().y + 100.0;

    menu->AddObject(new Button(play_position,     play_text,     bind(MainMenuPlay, main_menu, _1)));
    menu->AddObject(new Button(settings_position, settings_text, bind(MainMenuSettings, main_menu, _1)));
    menu->AddObject(new Button(credits_position,  credits_text,  bind(MainMenuCredits, main_menu, _1)));
    menu->AddObject(new Button(exit_position,     exit_text,     bind(SceneExit, main_menu, _1)));

#ifdef DEBUG
    Drawable* debug_text    = ResourceManager::GetLanguageWord("DebugStage")->CreateLabel();
    ugdk::math::Vector2D debug_position;
    debug_position.x = debug_text->size().x * 0.6;
    debug_position.y = 50.0;
    menu->AddObject(new Button(debug_position,    debug_text,    bind(MainMenuDebugPlay, main_menu, _1)));
#endif

    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    main_menu->interface_node()->AddChild(menu->node());
    main_menu->AddEntity(menu);

    return main_menu;
}

struct SettingsFunction {
    std::string name;
    std::tr1::function<void (utils::Settings*, int)> function;
    std::vector<std::string> values;
};
static void fillSettingsFunction(SettingsFunction* sf) {
    sf[0].name = "Resolution";
    sf[0].function = mem_fn(&Settings::set_resolution);
    const Vector2D *resolutions = Settings::reference()->ResolutionList();
    for (int i = 0; i < Settings::NUM_RESOLUTIONS; ++i) {
        std::ostringstream stm;
        stm << static_cast<int>(resolutions[i].x) << "x" << static_cast<int>(resolutions[i].y);
        sf[0].values.push_back(stm.str());
    }

    sf[1].name = "Fullscreen";
    sf[1].function = mem_fn(&Settings::set_fullscreen);
    sf[1].values.push_back("Off");
    sf[1].values.push_back("On");

    sf[2].name = "Music";
    sf[2].function = mem_fn(&Settings::set_background_music);
    sf[2].values.push_back("Off");
    sf[2].values.push_back("On");

    sf[3].name = "Sound Effects";
    sf[3].function = mem_fn(&Settings::set_sound_effects);
    sf[3].values.push_back("Off");
    sf[3].values.push_back("On");

    sf[4].name = "Language";
    sf[4].function = mem_fn(&Settings::set_language);
    const std::string *language_name = Settings::reference()->LanguageNameList();
    sf[4].values.push_back(language_name[0]);
    sf[4].values.push_back(language_name[1]);
}

struct ConveninentSettingsData {
    Node **nodes_[5];
    int sprites_active_[5];
    SettingsFunction setting_functions_[5];
    std::map<const UIElement*, int> indices_;

    ConveninentSettingsData(Node* node) {
        fillSettingsFunction(this->setting_functions_);

        Settings* settings_ = Settings::reference();

        this->sprites_active_[0] = Settings::reference()->resolution();
        this->sprites_active_[1] = settings_->fullscreen();
        this->sprites_active_[2] = settings_->background_music();
        this->sprites_active_[3] = settings_->sound_effects();
        this->sprites_active_[4] = settings_->language();

        double second_column_x = VIDEO_MANAGER()->video_size().x * 0.8;
    
        for(int i = 0; i < 5; ++i) {
            size_t size = this->setting_functions_[i].values.size();
            this->nodes_[i] = new Node*[size];
            for(size_t j = 0; j < size; ++j) {
                ugdk::LanguageWord* word = ResourceManager::GetLanguageWord(this->setting_functions_[i].values[j]);
                Drawable *img;
                if(word)
                    img = word->CreateLabel();
                else
                    img = new ugdk::graphic::Label(convertFromString(this->setting_functions_[i].values[j]), 
                                                   TEXT_MANAGER()->GetFont("FontB"));
                img->set_hotspot(Drawable::CENTER);
                this->nodes_[i][j] = new Node(img);
                this->nodes_[i][j]->geometry().set_offset(Vector2D(second_column_x, 70.0 * (i + 1)));
                node->AddChild(this->nodes_[i][j]);
                if ( static_cast<int>(j) != this->sprites_active_[i] ) this->nodes_[i][j]->effect().set_visible(false);
            }
        }
    }
    ~ConveninentSettingsData() {
        for(int i = 0; i < 5; ++i)
            delete nodes_[i];
    }
};

static void ChangeSetting(std::tr1::shared_ptr<ConveninentSettingsData> data, int modifier, const UIElement * source) {
    if(data->indices_.find(source) == data->indices_.end()) return;
    int value = data->indices_[source];
    int max_val = (int) data->setting_functions_[value].values.size();
    std::tr1::function<void (utils::Settings*, int)> settingsfunc = data->setting_functions_[value].function;

    data->nodes_[value][data->sprites_active_[value]]->effect().set_visible(false);
    data->sprites_active_[value] = (data->sprites_active_[value] + modifier) % max_val;
    if (data->sprites_active_[value] < 0) data->sprites_active_[value] += max_val;

    settingsfunc(Settings::reference(), data->sprites_active_[value]);

    data->nodes_[value][data->sprites_active_[value]]->effect().set_visible(true);
}

static void ElementPress(std::tr1::shared_ptr<ConveninentSettingsData> data, const Button * source) {
    ChangeSetting(data, +1, source);
}

static void PressArrow(std::tr1::shared_ptr<ConveninentSettingsData> data, int modifier, Menu* menu) {
    ChangeSetting(data, modifier, menu->focused_element());
}

static void ApplySettings(const Button * source) {
    Settings::reference()->WriteToDisk();
    utils::LevelManager::reference()->QueueRestartGame();
    ugdk::Engine::reference()->quit();
}

Scene* MenuBuilder::SettingsMenu() const {
    ugdk::action::Scene* settings_menu = new Scene();
    ugdk::math::Vector2D origin(0.0, 0.0), target = VIDEO_MANAGER()->video_size();
    ugdk::ikdtree::Box<2> box(origin.val, target.val);
    utils::MenuImageFactory mif;

    Menu* menu = new Menu(box, Vector2D(0.0, 0.0), ugdk::graphic::Drawable::LEFT);
    for(int i = 0; i < 2; ++i) {
        ugdk::graphic::Sprite* sprite = mif.HorusEye();
        settings_menu->media_manager().AddPlayer(&sprite->animation_player());
        menu->SetOptionDrawable(sprite, i);
    }

    std::tr1::shared_ptr<ConveninentSettingsData> data(new ConveninentSettingsData(settings_menu->interface_node()));
    double left_column = target.x * 0.15;

    for (int i = 0; i < 5; ++i) {
        Drawable* img = ResourceManager::GetLanguageWord(data->setting_functions_[i].name)->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * (i + 1));
        Button* uie = new Button(pos, img, bind(ElementPress, data, _1));
        data->indices_[uie] = i;
        menu->AddObject(uie);
    }

    {   Drawable* img = ResourceManager::GetLanguageWord("Apply")->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * 7);
        menu->AddObject(new Button(pos, img, ApplySettings)); }

    {   Drawable* img = ResourceManager::GetLanguageWord("Exit")->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * 8);
        menu->AddObject(new Button(pos, img, bind(SceneExit, settings_menu, _1))); }

    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    menu->AddCallback(ugdk::input::K_RIGHT , bind(PressArrow, data, +1, _1));
    menu->AddCallback(ugdk::input::K_LEFT  , bind(PressArrow, data, -1, _1));

    settings_menu->interface_node()->AddChild(menu->node());
    settings_menu->AddEntity(menu);
    return settings_menu;
}

}
