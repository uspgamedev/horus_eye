
#include "common.h"
#include "frontend/nativescenes.h"

#include "game/constants.h"
#include "game/scenes/scrollingimagescene.h"
#include "game/utils/settings.h"

#include <ugdk/audio/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/resource/module.h>
#include <ugdk/system/compatibility.h>
#include <ugdk/text/module.h>
#include <ugdk/text/label.h>
#include <ugdk/text/textbox.h>
#include <ugdk/text/languageword.h>
#include <ugdk/ui/menu.h>
#include <ugdk/ui/node.h>
#include <ugdk/ui/button.h>
#include <ugdk/ui/drawable/texturedrectangle.h>

namespace frontend {
namespace nativescenes {

using ugdk::math::Vector2D;
using ugdk::ui::Menu;
using ugdk::ui::Node;
using ugdk::ui::Drawable;
using ugdk::ui::Button;

namespace {
    void MainMenuCredits(const Button * source) {
        using ugdk::text::TextBox;

        ugdk::text::LanguageWord* langword = ugdk::resource::GetLanguageWord("CreditsFile");
        auto textbox = ugdk::MakeUnique<TextBox>(langword->text(), ugdk::graphic::manager()->screen()->size().x, ugdk::text::manager()->GetFont(langword->font()));
        textbox->set_ident_style(TextBox::CENTER);
        auto scroll = ugdk::MakeUnique<scene::ScrollingImageScene>(nullptr, std::move(textbox), 55);
        if (utils::Settings::reference()->background_music())
            scroll->set_background_music(ugdk::audio::manager()->LoadMusic("musics/action_game_theme.ogg"));
        ugdk::system::PushScene(std::move(scroll));
    }
}

std::unique_ptr<ugdk::action::Scene> HomeScene() {
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->screen()->size();

    std::unique_ptr<Menu> menu = BaseBuildMenu();
    menu->set_identifier("Main Menu");

    auto logo = ugdk::MakeUnique<ugdk::ui::TexturedRectangle>(ugdk::resource::GetTextureFromFile("images/logo_560x334_black.png"));
    logo->set_hotspot(ugdk::ui::HookPoint::TOP);
    auto logo_node = std::make_shared<Node>(std::move(logo));
    logo_node->geometry().set_offset(Vector2D(target.x * 0.5, 0.0));

    auto version = ugdk::MakeUnique<ugdk::text::Label>(constants::version(), ugdk::text::manager()->GetFont("FontD"));
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

    menu->AddObject(new Button(play_position,     std::unique_ptr<Drawable>(play_text),     [](const Button*) { ugdk::system::PushSceneFactory(CampaignSelectionScene); }));
    menu->AddObject(new Button(settings_position, std::unique_ptr<Drawable>(settings_text), [](const Button*) { ugdk::system::PushSceneFactory(SettingsScene); }));
    menu->AddObject(new Button(credits_position,  std::unique_ptr<Drawable>(credits_text),  MainMenuCredits));
    menu->AddObject(new Button(exit_position,     std::unique_ptr<Drawable>(exit_text),     FinishOwner));

    return std::move(menu);
}

} // namespace nativescenes
} // namespace frontend
