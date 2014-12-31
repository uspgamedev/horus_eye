
#include "common.h"
#include "frontend/nativescenes.h"

#include "game/campaigns/campaigndescriptor.h"
#include "game/campaigns/campaign.h"
#include "game/constants.h"

#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/resource/module.h>
#include <ugdk/system/compatibility.h>
#include <ugdk/text/module.h>
#include <ugdk/text/label.h>
#include <ugdk/text/languageword.h>
#include <ugdk/ui/button.h>
#include <ugdk/ui/menu.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/text/languageword.h>

#include <sstream>
#include <functional>
#include <memory>
#include <dirent.h>

namespace frontend {
namespace nativescenes {

using ugdk::ui::Menu;
using ugdk::ui::Drawable;
using ugdk::ui::Button;
using ugdk::text::Label;

std::unique_ptr<ugdk::action::Scene> CampaignSelectionScene() {
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->screen()->size();

    std::unique_ptr<Menu> menu = BaseBuildMenu(ugdk::ui::HookPoint::LEFT);
    menu->set_identifier("Campaign Menu");

    double y = 100.0;

    for (const campaigns::CampaignDescriptor& campaign_descriptor : campaigns::FindAllCampaignDescriptorsAt(constants::data_location() + "scripts/campaigns")) {
        if (!campaign_descriptor.playable()) continue;

        menu->AddObject(
            new Button(
            ugdk::math::Vector2D(200.0, y),
            ugdk::MakeUnique<Label>(campaign_descriptor.name(), ugdk::text::manager()->GetFont("FontB")),
            [campaign_descriptor](const Button* btn) {
                btn->owner()->Finish();
                ugdk::system::PushScene(ugdk::MakeUnique<campaigns::Campaign>(campaign_descriptor));
        }));

        y += 50.0;
    }

    menu->AddObject(new Button(ugdk::math::Vector2D(200.0, ugdk::graphic::manager()->screen()->size().y - 100.0),
                               std::unique_ptr<Drawable>(ugdk::resource::GetLanguageWord("Exit")->CreateLabel()),
                               FinishOwner));

    return std::move(menu);
}

} // namespace nativescenes
} // namespace frontend
