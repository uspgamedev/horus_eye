#include "game/campaigns/campaign.h"

#include "game/utils/levelloader.h"
#include "game/scenes/world.h"
#include "game/builders/herobuilder.h"

#include <ugdk/resource/module.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/system/engine.h>

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
}

Campaign* Campaign::CurrentCampaign() {
    return current_campaign_;
}

Campaign::Campaign(const CampaignDescriptor& d)
: current_level_(nullptr)
, descriptor_(d)
, node_(new ugdk::graphic::Node)
{
    set_visible(false);

    auto loading_image = ugdk::resource::GetLanguageWord("Loading")->CreateLabel();
    loading_image->set_hotspot(ugdk::graphic::Drawable::BOTTOM_RIGHT);
    auto loading = new ugdk::graphic::Node(loading_image);
    loading->geometry().set_offset(ugdk::graphic::manager()->canvas()->size() - ugdk::math::Vector2D(10.0, 10.0));
    node_->AddChild(loading);
}

Campaign::~Campaign() {}

void Campaign::Focus() {
    Scene::Focus();
    set_visible(true);
}

void Campaign::DeFocus() {
    Scene::DeFocus();
    set_visible(false);
}

void Campaign::End() {
}
    
void Campaign::LoadLevel(const std::string& level_name) {
    std::string level_path = descriptor_.script_path() + "." + level_name;
    
    utils::LoadLevel(SCRIPT_MANAGER()->LoadModule(level_path), level_path, &current_level_);

    /*if (!current_level_) {
        ugdk::system::PushScene(new ImageScene(NULL, new ugdk::graphic::Label(
            "Error loading level '" + (level_name)+"' from campaign '" + (current_campaign_)+"'.",
            TEXT_MANAGER()->current_font()
            )));
        if (ugdk::input::manager()->keyboard().IsDown(ugdk::input::Keycode::ESCAPE))
            loading_->Finish();
        return;
    }*/
    current_level_->SetHero(builder::HeroBuilder::Kha());
    ugdk::system::PushScene(current_level_);
    current_level_->Start();
}

} // namespace campaigns
