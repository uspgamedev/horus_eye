
#include "commonmenu.h"
#include "frontend/nativebuilders.h"
#include "frontend/scenes/campaigndisplay.h"

#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/resource/module.h>
#include <ugdk/ui/button.h>
#include <ugdk/ui/menu.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/text/languageword.h>

namespace frontend {
namespace nativebuilders {

using ugdk::ui::Menu;
using ugdk::ui::Drawable;
using ugdk::ui::Button;

std::unique_ptr<ugdk::action::Scene> PauseScene() {
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
        scenes::CampaignDisplay::Current()->Finish();
    }));

    return std::move(menu);
}

} // namespace nativebuilders
} // namespace frontend
