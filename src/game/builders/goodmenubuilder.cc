#include <sstream>
#include <functional>
#include <memory>
#include <dirent.h>

#include <ugdk/action.h>
#include <ugdk/action/scene.h>
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/input/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/label.h>
#include <ugdk/ui/menu.h>
#include <ugdk/ui/button.h>
#include <ugdk/structure/intervalkdtree.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>

#include "goodmenubuilder.h"

#include "game/constants.h"
#include "game/scenes/world.h"
#include "game/utils/levelmanager.h"
#include "game/utils/menuimagefactory.h"
#include "game/utils/settings.h"

using std::bind;
using std::mem_fn;
using namespace std::placeholders;
using ugdk::action::Scene;
using ugdk::graphic::Drawable;
using ugdk::graphic::Node;
using ugdk::graphic::Label;
using ugdk::ui::Menu;
using ugdk::ui::Button;
using ugdk::ui::UIElement;
using utils::Settings;
using ugdk::resource::GetLanguageWord;

static std::wstring convertFromString(const std::string& str) {
    std::wstring str2(str.length(), L' '); // Make room for characters
    std::copy(str.begin(), str.end(), str2.begin());
    return str2;
}

namespace builder {

void MenuFocus(Scene* scene){ 
    scene->set_active(true);
    scene->set_visible(true);
}

void MenuDeFocus(Scene* scene){ 
    scene->set_active(false);
    scene->set_visible(false);
}

void MainMenuCredits(Scene* menu, const Button * source) {
    utils::LevelManager::reference()->ShowCredits();
}

#ifdef DEBUG
void MainMenuDebugPlay(Scene* menu, const Button * source) {
    utils::LevelManager::reference()->DebugLoadSpecificLevel("debug_level");
}
#endif

Scene* PauseMenu() {
    ugdk::action::Scene* pause_menu = new Scene();
    pause_menu->set_focus_callback(MenuFocus);
    pause_menu->set_defocus_callback(MenuDeFocus);
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->video_size();
    utils::MenuImageFactory mif;

    Menu* menu = new Menu(ugdk::structure::Box<2>(origin, target), Vector2D(0.0, 0.0), ugdk::graphic::Drawable::CENTER);
    for(int i = 0; i < 2; ++i) {
        ugdk::graphic::Sprite* sprite = mif.HorusEye();
        pause_menu->media_manager().AddPlayer(&sprite->animation_player());
        menu->SetOptionDrawable(sprite, i);
    }

    Drawable* cont_text = ugdk::resource::GetLanguageWord("Continue")->CreateLabel();
    Drawable* exit_text = ugdk::resource::GetLanguageWord("Return to Menu")->CreateLabel();

    ugdk::math::Vector2D cont_position = target * 0.5;
    cont_position.y -= cont_text->size().y;

    ugdk::math::Vector2D exit_position = target * 0.5;
    exit_position.y += exit_text->size().y;

    menu->AddObject(new Button(cont_position, cont_text, [pause_menu](const Button*) { pause_menu->Finish(); }));
    menu->AddObject(new Button(exit_position, exit_text, [pause_menu](const Button*) { 
        pause_menu->Finish();
        WORLD()->FinishLevel(utils::LevelManager::FINISH_QUIT);
    }));

    pause_menu->StopsPreviousMusic(false);
    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    pause_menu->AddEntity(menu);
    pause_menu->set_render_function(std::bind(std::mem_fn(&ugdk::graphic::Node::Render), menu->node(), _1, _2));

    //TODO: solid rectangle no longer exists
    //ugdk::graphic::SolidRectangle* bg = new ugdk::graphic::SolidRectangle(target);
    //bg->set_color(ugdk::Color(0.5, 0.5, 0.5, 0.5));
    //pause_menu->interface_node()->set_drawable(bg);

    return pause_menu;
}

Scene* CampaignMenu() {
    ugdk::action::Scene* mission_menu = new Scene();
    mission_menu->set_focus_callback(MenuFocus);
    mission_menu->set_defocus_callback(MenuDeFocus);
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->video_size();
    utils::MenuImageFactory mif;

    Menu* menu = new Menu(ugdk::structure::Box<2>(origin, target), Vector2D(0.0, 0.0), ugdk::graphic::Drawable::LEFT);
    for(int i = 0; i < 2; ++i) {
        ugdk::graphic::Sprite* sprite = mif.HorusEye();
        mission_menu->media_manager().AddPlayer(&sprite->animation_player());
        menu->SetOptionDrawable(sprite, i);
    }

    double y = 100.0;

    //
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir((constants::data_location() + "scripts/campaigns").c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_name[0] == '.') continue;

            using ugdk::script::VirtualObj;
            
            std::string campaign_module_name = std::string(ent->d_name);

            VirtualObj campaign_module = SCRIPT_MANAGER()->LoadModule("campaigns." + campaign_module_name + ".descriptor");
            if(!campaign_module) continue;
            if(!campaign_module["playable"] || !campaign_module["playable"].value<bool>()) continue;
            if(!campaign_module["name"]) continue;

            std::wstring name(convertFromString(campaign_module["name"].value<std::string>()));

            menu->AddObject(
                new Button(
                    Vector2D(200.0, y),
                    new Label(name, TEXT_MANAGER()->GetFont("FontB")),
                    [campaign_module_name,campaign_module,mission_menu](const Button*) {
                        mission_menu->Finish(); 
                        utils::LevelManager* levelmanager = utils::LevelManager::reference();

                        std::vector<std::string> list; 
                        for(const auto& level : campaign_module["level_list"].value<VirtualObj::Vector>())
                            list.push_back(level.value<std::string>());

                        levelmanager->ChangeCampaign(campaign_module_name, list);
                        levelmanager->ShowIntro();
            }));

            y += 50.0;
        }
        closedir (dir);
    } else {
        fprintf(stderr, "NAO ABRUI A PASTA");
    }

    menu->AddObject(new Button(Vector2D(200.0, ugdk::graphic::manager()->video_size().y - 100.0),
                               GetLanguageWord("Exit")->CreateLabel(),
                               [mission_menu](const Button*) { mission_menu->Finish(); }));

    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    mission_menu->AddEntity(menu);
    mission_menu->set_render_function(std::bind(std::mem_fn(&ugdk::graphic::Node::Render), menu->node(), _1, _2));

    return mission_menu;
}

Scene* MainMenu() {
    ugdk::action::Scene* main_menu = new Scene();
    main_menu->set_focus_callback(MenuFocus);
    main_menu->set_defocus_callback(MenuDeFocus);

    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->video_size();
    utils::MenuImageFactory mif;

    ugdk::graphic::Drawable *logo = new ugdk::graphic::TexturedRectangle(ugdk::resource::GetTextureFromFile("images/logo_560x334_black.png"));
    logo->set_hotspot(ugdk::graphic::Drawable::TOP);
    Node* logo_node = new Node(logo);
    logo_node->geometry().set_offset(Vector2D(target.x * 0.5, 0.0));

    ugdk::graphic::Drawable *version = new ugdk::graphic::Label(constants::version(), TEXT_MANAGER()->GetFont("FontD"));
    version->set_hotspot(ugdk::graphic::Drawable::BOTTOM_LEFT);
    Node* version_node = new Node(version);
    version_node->geometry().set_offset(Vector2D(10.0, target.y - 10.0));

    ugdk::graphic::Drawable *developed_by = new ugdk::graphic::TexturedRectangle(ugdk::resource::GetTextureFromFile("images/developed_by_uspgamedev1.png"));
    developed_by->set_hotspot(ugdk::graphic::Drawable::BOTTOM_RIGHT);
    Node* developed_by_node = new Node(developed_by);
    developed_by_node->geometry().set_offset(ugdk::graphic::manager()->video_size() + Vector2D(-15.0, 0.0));

    Menu* menu = new Menu(ugdk::structure::Box<2>(origin, target), Vector2D(0.0, 0.0), ugdk::graphic::Drawable::CENTER);
    for(int i = 0; i < 2; ++i) {
        ugdk::graphic::Sprite* sprite = mif.HorusEye();
        main_menu->media_manager().AddPlayer(&sprite->animation_player());
        menu->SetOptionDrawable(sprite, i);
    }

    // The scene's drawables
    menu->node()->AddChild(logo_node);
    menu->node()->AddChild(version_node);
    menu->node()->AddChild(developed_by_node);
    // =======

    Drawable* play_text     = ugdk::resource::GetLanguageWord("Play")->CreateLabel();
    Drawable* settings_text = ugdk::resource::GetLanguageWord("Settings")->CreateLabel();
    Drawable* credits_text  = ugdk::resource::GetLanguageWord("Credits")->CreateLabel();
    Drawable* exit_text     = ugdk::resource::GetLanguageWord("Exit")->CreateLabel();

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

    menu->AddObject(new Button(play_position,     play_text,     [](const Button*) { ugdk::system::PushScene(CampaignMenu()); }));
    menu->AddObject(new Button(settings_position, settings_text, [](const Button*) { ugdk::system::PushScene(SettingsMenu()); }));
    menu->AddObject(new Button(credits_position,  credits_text,  [](const Button*) { utils::LevelManager::reference()->ShowCredits(); }));
    menu->AddObject(new Button(exit_position,     exit_text,     [main_menu](const Button*) { main_menu->Finish(); }));

#ifdef DEBUG
    Drawable* debug_text    = ugdk::resource::GetLanguageWord("DebugStage")->CreateLabel();
    ugdk::math::Vector2D debug_position;
    debug_position.x = debug_text->size().x * 0.6;
    debug_position.y = 50.0;
    menu->AddObject(new Button(debug_position, debug_text, [](const Button*) { utils::LevelManager::reference()->DebugLoadSpecificLevel("debug_level"); }));
#endif

    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    
    main_menu->AddEntity(menu);
    main_menu->set_render_function(std::bind(std::mem_fn(&ugdk::graphic::Node::Render), menu->node(), _1, _2));

    return main_menu;
}

struct SettingsFunction {
    std::string name;
    std::function<void (utils::Settings*, int)> function;
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

        double second_column_x = ugdk::graphic::manager()->video_size().x * 0.8;
    
        for(int i = 0; i < 5; ++i) {
            size_t size = this->setting_functions_[i].values.size();
            this->nodes_[i] = new Node*[size];
            for(size_t j = 0; j < size; ++j) {
                ugdk::LanguageWord* word = ugdk::resource::GetLanguageWord(this->setting_functions_[i].values[j]);
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

static void ChangeSetting(std::shared_ptr<ConveninentSettingsData> data, int modifier, const UIElement * source) {
    if(data->indices_.find(source) == data->indices_.end()) return;
    int value = data->indices_[source];
    int max_val = (int) data->setting_functions_[value].values.size();
    std::function<void (utils::Settings*, int)> settingsfunc = data->setting_functions_[value].function;

    data->nodes_[value][data->sprites_active_[value]]->effect().set_visible(false);
    data->sprites_active_[value] = (data->sprites_active_[value] + modifier) % max_val;
    if (data->sprites_active_[value] < 0) data->sprites_active_[value] += max_val;

    settingsfunc(Settings::reference(), data->sprites_active_[value]);

    data->nodes_[value][data->sprites_active_[value]]->effect().set_visible(true);
}

static void ElementPress(std::shared_ptr<ConveninentSettingsData> data, const Button * source) {
    ChangeSetting(data, +1, source);
}

static void PressArrow(std::shared_ptr<ConveninentSettingsData> data, int modifier, Menu* menu) {
    ChangeSetting(data, modifier, menu->focused_element());
}

static void ApplySettings(const Button * source) {
    Settings::reference()->WriteToDisk();
    utils::LevelManager::reference()->QueueRestartGame();
    ugdk::system::Quit();
}

Scene* SettingsMenu() {
    ugdk::action::Scene* settings_menu = new Scene();
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->video_size();
    utils::MenuImageFactory mif;

    Menu* menu = new Menu(ugdk::structure::Box<2>(origin, target), Vector2D(0.0, 0.0), ugdk::graphic::Drawable::LEFT);
    for(int i = 0; i < 2; ++i) {
        ugdk::graphic::Sprite* sprite = mif.HorusEye();
        settings_menu->media_manager().AddPlayer(&sprite->animation_player());
        menu->SetOptionDrawable(sprite, i);
    }

    std::shared_ptr<ConveninentSettingsData> data(new ConveninentSettingsData(menu->node()));
    double left_column = target.x * 0.15;

    for (int i = 0; i < 5; ++i) {
        Drawable* img = ugdk::resource::GetLanguageWord(data->setting_functions_[i].name)->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * (i + 1));
        Button* uie = new Button(pos, img, bind(ElementPress, data, _1));
        data->indices_[uie] = i;
        menu->AddObject(uie);
    }

    {   Drawable* img = ugdk::resource::GetLanguageWord("Apply")->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * 7);
        menu->AddObject(new Button(pos, img, ApplySettings)); }

    {   Drawable* img = ugdk::resource::GetLanguageWord("Exit")->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * 8);
        menu->AddObject(new Button(pos, img, [settings_menu](const Button*) { settings_menu->Finish(); })); }

    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::Menu::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, ugdk::ui::Menu::INTERACT_MENU);
    menu->AddCallback(ugdk::input::K_RIGHT , bind(PressArrow, data, +1, _1));
    menu->AddCallback(ugdk::input::K_LEFT  , bind(PressArrow, data, -1, _1));

    settings_menu->AddEntity(menu);
    settings_menu->set_render_function(std::bind(std::mem_fn(&ugdk::graphic::Node::Render), menu->node(), _1, _2));
    return settings_menu;
}

}
