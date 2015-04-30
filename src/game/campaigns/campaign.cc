#include "game/campaigns/campaign.h"

#include "game/utils/levelloader.h"
#include "game/core/world.h"
#include "game/builders/herobuilder.h"
#include "game/context.h"
#include "game/campaigns/exceptions.h"

#include "communication/direct.h"

#include <ugdk/resource/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/system/engine.h>

using namespace ugdk;
using namespace ugdk::action;
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
    if (current_level_)
        delete current_level_;
    current_campaign_ = nullptr;
}

void Campaign::Start() {
    (implementation_ | "Start")(this);
}

void Campaign::End() {
    communication::notify::ChangeMusic("");
    (implementation_ | "End")(this);
}
   
void Campaign::InformSceneFinished() {
    auto level_to_delete = current_level_;
    current_level_ = nullptr;
    AddTask(ugdk::system::Task([this, level_to_delete](double) {
        (implementation_ | "OnSceneFinished")(this);
        delete level_to_delete;
        return false;
    }, 0.0));
}
    
bool Campaign::LoadLevel(const std::string& level_name) {
    std::string level_path = descriptor_.script_path() + "." + level_name;
    current_level_ = utils::LoadLevel(SCRIPT_MANAGER()->LoadModule(level_path), level_path);

    if (current_level_) {
        current_level_->Start(this);
        communication::notify::CampaignLevelLoaded();
        return true;
    }
    else {
        communication::notify::CampaignFailedToLoadLevel("Error loading level '" + (level_name)+"' from campaign '" + descriptor_.name() + "'.");
        return false;
    }
}

} // namespace campaigns
