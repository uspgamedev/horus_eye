#include "game/campaigns/campaign.h"

#include "game/utils/levelloader.h"
#include "game/scenes/lightrendering.h"
#include "game/scenes/world.h"
#include "game/builders/herobuilder.h"
#include "game/context.h"
#include "game/campaigns/exceptions.h"

#include <ugdk/resource/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/system/engine.h>

using namespace ugdk;
using namespace ugdk::action;
using namespace scene;
using namespace sprite;

namespace campaigns {

namespace {
    Campaign* current_campaign_ = nullptr;
}

Campaign* Campaign::CurrentCampaign() {
    return current_campaign_;
}

Campaign::Campaign(const CampaignDescriptor& d)
: current_level_(nullptr)
, descriptor_(d)
{
    if (current_campaign_)
        throw AnotherCampaignExists();
    current_campaign_ = this;

    implementation_ = SCRIPT_MANAGER()->LoadModule(descriptor_.script_path() + ".main")["new"](this);
}

Campaign::~Campaign() {
    current_campaign_ = nullptr;
}

void Campaign::Focus() {
    Scene::Focus();
    (implementation_ | "Focus")(this);
}

void Campaign::DeFocus() {
    Scene::DeFocus();
    (implementation_ | "DeFocus")(this);
}

void Campaign::End() {
    (implementation_ | "End")(this);
}
    
void Campaign::InformLevelFinished() {
    current_level_ = nullptr;
}
    
bool Campaign::LoadLevel(const std::string& level_name) {
    std::string level_path = descriptor_.script_path() + "." + level_name;
    current_level_ = utils::LoadLevel(SCRIPT_MANAGER()->LoadModule(level_path), level_path);

    if (current_level_) {
        auto lr = ugdk::MakeUnique<scene::LightRendering>(current_level_);
        current_level_->set_light_rendering(lr.get());

        ugdk::system::PushScene(std::move(lr));
        ugdk::system::PushScene(std::unique_ptr<Scene>(current_level_));
        current_level_->Start(this);
        return true;
    }
    else {
        context::ShowTextAsScene("Error loading level '" + (level_name)+ "' from campaign '" + descriptor_.name() +"'.");
        return false;
    }
}

} // namespace campaigns
