#include "common.h"
#include "frontend/nativescenes.h"

#include "game/initializer.h"
#include "game/utils/settings.h"

#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/input/keycode.h>
#include <ugdk/resource/module.h>
#include <ugdk/text/module.h>
#include <ugdk/text/label.h>
#include <ugdk/ui/button.h>
#include <ugdk/ui/menu.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/ui/uielement.h>
#include <ugdk/ui/node.h>
#include <ugdk/text/languageword.h>

#include <sstream>

namespace frontend {
namespace nativescenes {

using ugdk::ui::Menu;
using ugdk::ui::Drawable;
using ugdk::ui::Button;
using ugdk::ui::UIElement;
using ugdk::ui::Node;
using ugdk::text::Label;
using ugdk::math::Vector2D;
using utils::Settings;
using namespace std::placeholders;

namespace {

    struct SettingsFunction {
        std::string name;
        std::function<void(utils::Settings*, int)> function;
        std::vector<std::string> values;
    };
    void fillSettingsFunction(SettingsFunction* sf) {
        sf[0].name = "Resolution";
        sf[0].function =std:: mem_fn(&Settings::set_resolution);
        const ugdk::math::Integer2D* resolutions = Settings::reference()->ResolutionList();
        for (int i = 0; i < Settings::NUM_RESOLUTIONS; ++i) {
            std::ostringstream stm;
            stm << resolutions[i].x << "x" << resolutions[i].y;
            sf[0].values.push_back(stm.str());
        }

        sf[1].name = "Fullscreen";
        sf[1].function = std::mem_fn(&Settings::set_fullscreen);
        sf[1].values.push_back("Off");
        sf[1].values.push_back("On");

        sf[2].name = "Music";
        sf[2].function = std::mem_fn(&Settings::set_background_music);
        sf[2].values.push_back("Off");
        sf[2].values.push_back("On");

        sf[3].name = "Sound Effects";
        sf[3].function = std::mem_fn(&Settings::set_sound_effects);
        sf[3].values.push_back("Off");
        sf[3].values.push_back("On");

        sf[4].name = "Language";
        sf[4].function = std::mem_fn(&Settings::set_language);
        const std::string *language_name = Settings::reference()->LanguageNameList();
        sf[4].values.push_back(language_name[0]);
        sf[4].values.push_back(language_name[1]);

        sf[5].name = "VSync";
        sf[5].function = std::mem_fn(&Settings::set_vsync);
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
                for (size_t j = 0; j < size; ++j) {
                    ugdk::text::LanguageWord* word = ugdk::resource::GetLanguageWord(this->setting_functions_[i].values[j]);
                    std::unique_ptr<Drawable> img;
                    if (word)
                        img.reset(word->CreateLabel());
                    else
                        img.reset(new Label(this->setting_functions_[i].values[j], ugdk::text::manager()->GetFont("FontB")));

                    img->set_hotspot(ugdk::ui::HookPoint::CENTER);
                    this->nodes_[i][j].reset(new Node(std::move(img)));
                    this->nodes_[i][j]->geometry().set_offset(Vector2D(second_column_x, 70.0 * (i + 1)));
                    node->AddChild(this->nodes_[i][j]);
                    if (static_cast<int>(j) != this->sprites_active_[i]) this->nodes_[i][j]->effect().set_visible(false);
                }
            }
        }
        ~ConveninentSettingsData() {
            for (int i = 0; i < NUM_SETTINGS; ++i)
                delete nodes_[i];
        }
    };

    void ChangeSetting(std::shared_ptr<ConveninentSettingsData> data, int modifier, const UIElement * source) {
        if (data->indices_.find(source) == data->indices_.end()) return;
        int value = data->indices_[source];
        int max_val = (int)data->setting_functions_[value].values.size();
        std::function<void(utils::Settings*, int)> settingsfunc = data->setting_functions_[value].function;

        data->nodes_[value][data->sprites_active_[value]]->effect().set_visible(false);
        data->sprites_active_[value] = (data->sprites_active_[value] + modifier) % max_val;
        if (data->sprites_active_[value] < 0) data->sprites_active_[value] += max_val;

        settingsfunc(utils::Settings::reference(), data->sprites_active_[value]);

        data->nodes_[value][data->sprites_active_[value]]->effect().set_visible(true);
    }

    void ElementPress(std::shared_ptr<ConveninentSettingsData> data, const Button * source) {
        ChangeSetting(data, +1, source);
    }

    void PressArrow(std::shared_ptr<ConveninentSettingsData> data, int modifier, Menu* menu) {
        ChangeSetting(data, modifier, menu->focused_element());
    }

    void ApplySettings(const Button * source) {
        utils::Settings::reference()->WriteToDisk();
        QueueRestartGame();
        ugdk::system::Suspend();
    }
}

std::unique_ptr<ugdk::action::Scene> SettingsScene() {
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->screen()->size();

    std::unique_ptr<Menu> menu = BaseBuildMenu();
    menu->set_identifier("Settings Menu");

    std::shared_ptr<ConveninentSettingsData> data(new ConveninentSettingsData(menu->node()));
    double left_column = target.x * 0.20;

    for (int i = 0; i < ConveninentSettingsData::NUM_SETTINGS; ++i) {
        Drawable* img = ugdk::resource::GetLanguageWord(data->setting_functions_[i].name)->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * (i + 1));
        Button* uie = new Button(pos, std::unique_ptr<Drawable>(img), std::bind(ElementPress, data, _1));
        data->indices_[uie] = i;
        menu->AddObject(uie);
    }

    {   Drawable* img = ugdk::resource::GetLanguageWord("Apply")->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * (ConveninentSettingsData::NUM_SETTINGS + 2));
        menu->AddObject(new Button(pos, std::unique_ptr<Drawable>(img), ApplySettings)); }

    {   Drawable* img = ugdk::resource::GetLanguageWord("Exit")->CreateLabel();
        ugdk::math::Vector2D pos = ugdk::math::Vector2D(left_column, 70.0 * (ConveninentSettingsData::NUM_SETTINGS + 3));
        menu->AddObject(new Button(pos, std::unique_ptr<Drawable>(img), FinishOwner)); }

    menu->AddCallback(ugdk::input::Keycode::RIGHT, std::bind(PressArrow, data, +1, _1));
    menu->AddCallback(ugdk::input::Keycode::LEFT, std::bind(PressArrow, data, -1, _1));

    return std::move(menu);
}

} // namespace nativescenes
} // namespace frontend
