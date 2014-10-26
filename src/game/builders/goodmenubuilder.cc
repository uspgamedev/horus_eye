#include "goodmenubuilder.h"

#include <ugdk/input/keycode.h>
#include <ugdk/action.h>
#include <ugdk/action/events.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/mediaplayer.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/audio/module.h>
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/input/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/spritetypes.h>
#include <ugdk/text/label.h>
#include <ugdk/text/textbox.h>
#include <ugdk/text/module.h>
#include <ugdk/structure/intervalkdtree.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/text/languageword.h>
#include <ugdk/ui/menu.h>
#include <ugdk/ui/button.h>
#include <ugdk/ui/node.h>
#include <ugdk/ui/drawable/texturedrectangle.h>

#include "game/initializer.h"
#include "game/constants.h"
#include "game/campaigns/campaign.h"
#include "game/campaigns/campaigndescriptor.h"
#include "game/scenes/world.h"
#include "game/scenes/scrollingimagescene.h"
#include "game/utils/menuimagefactory.h"
#include "game/utils/settings.h"

#include <sstream>
#include <functional>
#include <memory>
#include <dirent.h>


using std::bind;
using std::mem_fn;
using namespace std::placeholders;
using ugdk::action::Scene;
using ugdk::text::Label;
using ugdk::ui::Drawable;
using ugdk::ui::Node;
using ugdk::ui::Menu;
using ugdk::ui::Button;
using ugdk::ui::UIElement;
using utils::Settings;
using ugdk::resource::GetLanguageWord;

namespace builder {

namespace {

void MenuFocus(const ugdk::action::SceneFocusEvent& ev) {
    ev.scene->set_active(true);
    ev.scene->set_visible(true);
}

void MenuDeFocus(const ugdk::action::SceneDefocusEvent& ev) {
    ev.scene->set_active(false);
    ev.scene->set_visible(false);
}

void FinishOwner(const ugdk::ui::Button* button) {
    button->owner()->Finish();
}

void MainMenuCredits(const Button * source) {
    using ugdk::text::TextBox;

    ugdk::text::LanguageWord* langword = ugdk::resource::GetLanguageWord("CreditsFile");
    auto textbox = ugdk::MakeUnique<TextBox>(langword->text(), ugdk::graphic::manager()->screen()->size().x, ugdk::text::manager()->GetFont(langword->font()));
    textbox->set_ident_style(TextBox::CENTER);
    auto scroll = ugdk::MakeUnique<scene::ScrollingImageScene>(nullptr, std::move(textbox), 55);
    if (Settings::reference()->background_music())
        scroll->set_background_music(ugdk::audio::manager()->LoadMusic("musics/action_game_theme.ogg"));
    ugdk::system::PushScene(std::move(scroll));
}

class AnimationPlayerHolder : public ugdk::system::Task {
  public:
    AnimationPlayerHolder() : Task([this](double dt) {
        for (auto& p : players_)
            p->Update(dt);
    }) {}
    ~AnimationPlayerHolder() {}

    void Add(std::shared_ptr<ugdk::graphic::SpriteAnimationPlayer>& player) {
        players_.push_back(player);
    }

  private:
    std::list< std::shared_ptr<ugdk::graphic::SpriteAnimationPlayer> > players_;
};

std::unique_ptr<Menu> BaseBuildMenu(ugdk::ui::HookPoint hook = ugdk::ui::HookPoint::CENTER) {
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->screen()->size();
    utils::MenuImageFactory mif;

    auto holder = new AnimationPlayerHolder;
    auto menu = ugdk::MakeUnique<Menu>(ugdk::structure::Box<2>(origin, target), Vector2D(0.0, 0.0), hook);
    menu->event_handler().AddListener(MenuFocus);
    menu->event_handler().AddListener(MenuDeFocus);

    for (int i = 0; i < 2; ++i) {
        auto sprite = mif.HorusEye();
        menu->SetOptionDrawable(std::unique_ptr<Drawable>(sprite.first), i);
        holder->Add(sprite.second);
    }
    menu->AddTask(*holder);

    menu->AddCallback(ugdk::input::Keycode::ESCAPE, std::mem_fn(&ugdk::ui::Menu::Finish));
    menu->AddCallback(ugdk::input::Keycode::RETURN, std::mem_fn(&ugdk::ui::Menu::InteractWithFocused));

    menu->StopsPreviousMusic(false);
    return std::move(menu);
}

}

std::unique_ptr<Scene> PauseMenu() {
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->screen()->size();

    std::unique_ptr<Menu> menu = BaseBuildMenu();
    menu->set_identifier("Pause Menu");

    std::unique_ptr<Drawable> cont_text(ugdk::resource::GetLanguageWord("Continue")->CreateLabel());
    std::unique_ptr<Drawable> exit_text(ugdk::resource::GetLanguageWord("Return to Menu")->CreateLabel());

    ugdk::math::Vector2D cont_position = target * 0.5;
    cont_position.y -= cont_text->size().y;

    ugdk::math::Vector2D exit_position = target * 0.5;
    exit_position.y += exit_text->size().y;

    menu->AddObject(new Button(cont_position, std::move(cont_text), FinishOwner));
    menu->AddObject(new Button(exit_position, std::move(exit_text), [](const Button* btn) {
        btn->owner()->Finish();
        auto current_campaign = campaigns::Campaign::CurrentCampaign();
        current_campaign->current_level()->Finish();
        current_campaign->Finish();
    }));

    //TODO: solid rectangle no longer exists
    //ugdk::graphic::SolidRectangle* bg = new ugdk::graphic::SolidRectangle(target);
    //bg->set_color(ugdk::Color(0.5, 0.5, 0.5, 0.5));
    //pause_menu->interface_node()->set_drawable(bg);

    return std::move(menu);
}

std::unique_ptr<Scene> CampaignMenu() {
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->screen()->size();

    std::unique_ptr<Menu> menu = BaseBuildMenu(ugdk::ui::HookPoint::LEFT);
    menu->set_identifier("Campaign Menu");

    double y = 100.0;

    for (const campaigns::CampaignDescriptor& campaign_descriptor : campaigns::FindAllCampaignDescriptorsAt(constants::data_location() + "scripts/campaigns")) {
        if (!campaign_descriptor.playable()) continue;

        menu->AddObject(
            new Button(
            Vector2D(200.0, y),
            ugdk::MakeUnique<Label>(campaign_descriptor.name(), ugdk::text::manager()->GetFont("FontB")),
            [campaign_descriptor](const Button* btn) {
                btn->owner()->Finish();
                ugdk::system::PushScene(ugdk::MakeUnique<campaigns::Campaign>(campaign_descriptor));
        }));

        y += 50.0;
    }

    menu->AddObject(new Button(Vector2D(200.0, ugdk::graphic::manager()->screen()->size().y - 100.0),
                               std::unique_ptr<Drawable>(GetLanguageWord("Exit")->CreateLabel()),
                               [&menu](const Button*) { menu->Finish(); }));

    return std::move(menu);
}

std::unique_ptr<Scene> MainMenu() {
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->screen()->size();

    std::unique_ptr<Menu> menu = BaseBuildMenu();
    menu->set_identifier("Main Menu");

    auto logo = ugdk::MakeUnique<ugdk::ui::TexturedRectangle>(ugdk::resource::GetTextureFromFile("images/logo_560x334_black.png"));
    logo->set_hotspot(ugdk::ui::HookPoint::TOP);
    auto logo_node = std::make_shared<Node>(std::move(logo));
    logo_node->geometry().set_offset(Vector2D(target.x * 0.5, 0.0));

    auto version = ugdk::MakeUnique<Label>(constants::version(), ugdk::text::manager()->GetFont("FontD"));
    version->set_hotspot(ugdk::ui::HookPoint::BOTTOM_LEFT);
    auto version_node = std::make_shared<Node>(std::move(version));
    version_node->geometry().set_offset(Vector2D(10.0, target.y - 10.0));

    auto developed_by = ugdk::MakeUnique<ugdk::ui::TexturedRectangle>(ugdk::resource::GetTextureFromFile("images/developed_by_uspgamedev1.png"));
    developed_by->set_hotspot(ugdk::ui::HookPoint::BOTTOM_RIGHT);
    auto developed_by_node = std::make_shared<Node>(std::move(developed_by));
    developed_by_node->geometry().set_offset(ugdk::graphic::manager()->screen()->size() + Vector2D(-15.0, 0.0));

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

    menu->AddObject(new Button(play_position,     std::unique_ptr<Drawable>(play_text),     [](const Button*) { ugdk::system::PushSceneFactory(CampaignMenu); }));
    menu->AddObject(new Button(settings_position, std::unique_ptr<Drawable>(settings_text), [](const Button*) { ugdk::system::PushSceneFactory(SettingsMenu); }));
    menu->AddObject(new Button(credits_position,  std::unique_ptr<Drawable>(credits_text),  MainMenuCredits));
    menu->AddObject(new Button(exit_position,     std::unique_ptr<Drawable>(exit_text),     FinishOwner));

    return std::move(menu);
}

struct SettingsFunction {
    std::string name;
    std::function<void (utils::Settings*, int)> function;
    std::vector<std::string> values;
};
static void fillSettingsFunction(SettingsFunction* sf) {
    sf[0].name = "Resolution";
    sf[0].function = mem_fn(&Settings::set_resolution);
    const ugdk::math::Integer2D* resolutions = Settings::reference()->ResolutionList();
    for (int i = 0; i < Settings::NUM_RESOLUTIONS; ++i) {
        std::ostringstream stm;
        stm << resolutions[i].x << "x" << resolutions[i].y;
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
    
    sf[5].name = "VSync";
    sf[5].function = mem_fn(&Settings::set_vsync);
    sf[5].values.push_back("Off");
    sf[5].values.push_back("On");
}

struct ConveninentSettingsData {
    const static int NUM_SETTINGS = 6;
    std::shared_ptr<Node> *nodes_[NUM_SETTINGS];
    int sprites_active_[NUM_SETTINGS];
    SettingsFunction setting_functions_[NUM_SETTINGS];
    std::map<const UIElement*, int> indices_;

    ConveninentSettingsData(Node* node) {
        fillSettingsFunction(this->setting_functions_);

        Settings* settings_ = Settings::reference();

        this->sprites_active_[0] = Settings::reference()->resolution();
        this->sprites_active_[1] = settings_->fullscreen();
        this->sprites_active_[2] = settings_->background_music();
        this->sprites_active_[3] = settings_->sound_effects();
        this->sprites_active_[4] = settings_->language();
        this->sprites_active_[5] = settings_->vsync();

        double second_column_x = ugdk::graphic::manager()->screen()->size().x * 0.8;
    
        for (int i = 0; i < NUM_SETTINGS; ++i) {
            size_t size = this->setting_functions_[i].values.size();
            this->nodes_[i] = new std::shared_ptr<Node>[size];
            for(size_t j = 0; j < size; ++j) {
                ugdk::text::LanguageWord* word = ugdk::resource::GetLanguageWord(this->setting_functions_[i].values[j]);
                std::unique_ptr<Drawable> img;
                if(word)
                    img.reset(word->CreateLabel());
                else
                    img.reset(new Label(this->setting_functions_[i].values[j], ugdk::text::manager()->GetFont("FontB")));

                img->set_hotspot(ugdk::ui::HookPoint::CENTER);
                this->nodes_[i][j].reset(new Node(std::move(img)));
                this->nodes_[i][j]->geometry().set_offset(Vector2D(second_column_x, 70.0 * (i + 1)));
                node->AddChild(this->nodes_[i][j]);
                if ( static_cast<int>(j) != this->sprites_active_[i] ) this->nodes_[i][j]->effect().set_visible(false);
            }
        }
    }
    ~ConveninentSettingsData() {
        for (int i = 0; i < NUM_SETTINGS; ++i)
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
    QueueRestartGame();
    ugdk::system::Suspend();
}

std::unique_ptr<Scene> SettingsMenu() {
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->screen()->size();

    std::unique_ptr<Menu> menu = BaseBuildMenu();
    menu->set_identifier("Settings Menu");

    std::shared_ptr<ConveninentSettingsData> data(new ConveninentSettingsData(menu->node()));
    double left_column = target.x * 0.20;

    for (int i = 0; i < ConveninentSettingsData::NUM_SETTINGS; ++i) {
        Drawable* img = ugdk::resource::GetLanguageWord(data->setting_functions_[i].name)->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * (i + 1));
        Button* uie = new Button(pos, std::unique_ptr<Drawable>(img), bind(ElementPress, data, _1));
        data->indices_[uie] = i;
        menu->AddObject(uie);
    }

    {   Drawable* img = ugdk::resource::GetLanguageWord("Apply")->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * (ConveninentSettingsData::NUM_SETTINGS + 2));
        menu->AddObject(new Button(pos, std::unique_ptr<Drawable>(img), ApplySettings)); }

    {   Drawable* img = ugdk::resource::GetLanguageWord("Exit")->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * (ConveninentSettingsData::NUM_SETTINGS + 3));
        menu->AddObject(new Button(pos, std::unique_ptr<Drawable>(img), FinishOwner)); }

    menu->AddCallback(ugdk::input::Keycode::RIGHT , bind(PressArrow, data, +1, _1));
    menu->AddCallback(ugdk::input::Keycode::LEFT  , bind(PressArrow, data, -1, _1));

    return std::move(menu);
}

}
