#include "game/campaigns/campaign.h"

#include "game/utils/levelloader.h"
#include "game/scenes/world.h"
#include "game/builders/herobuilder.h"
#include "game/context.h"

#include <ugdk/resource/module.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/system/engine.h>
#include <ugdk/graphic/opengl/Exception.h>

#include "game/network/client/remotelevel.h"

using namespace ugdk;
using namespace ugdk::action;
using namespace scene;
using namespace sprite;

using ugdk::graphic::Drawable;
using ugdk::graphic::TexturedRectangle;
using ugdk::graphic::TextBox;

namespace campaigns {

namespace {
    Campaign* current_campaign_ = nullptr;
    network::RemoteLevel* THE_REMOTE_LEVEL_ = nullptr;
}

Campaign* Campaign::CurrentCampaign() {
    return current_campaign_;
}

Campaign::Campaign(const CampaignDescriptor& d)
: current_level_(nullptr)
, descriptor_(d)
{
    if (current_campaign_ != nullptr)
        throw love::Exception("Creating a new Campaign while another exists.");
    current_campaign_ = this;
    THE_REMOTE_LEVEL_ = new network::RemoteLevel;

    implementation_ = SCRIPT_MANAGER()->LoadModule(descriptor_.script_path() + ".main")["new"](this);
}

Campaign::~Campaign() {
    current_campaign_ = nullptr;
    delete THE_REMOTE_LEVEL_;
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
    utils::LoadLevel(SCRIPT_MANAGER()->LoadModule(level_path), level_path, &current_level_);

    if (current_level_) {
        ugdk::system::PushScene(current_level_);
        ugdk::system::PushScene(THE_REMOTE_LEVEL_);
        current_level_->Start(this);
        return true;
    }
    else {
        context::ShowTextAsScene("Error loading level '" + (level_name)+ "' from campaign '" + descriptor_.name() +"'.");
        return false;
    }
}

} // namespace campaigns
