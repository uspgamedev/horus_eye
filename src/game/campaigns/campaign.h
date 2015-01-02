#ifndef HORUSEYE_GAME_CAMPAIGNS_CAMPAIGN_H_
#define HORUSEYE_GAME_CAMPAIGNS_CAMPAIGN_H_

#include <ugdk/action/scene.h>

#include "game/campaigns/campaigndescriptor.h"
#include "game/campaigns.h"
#include "game/sprites.h"
#include "game/core.h"

#include <ugdk/script/virtualobj.h>
#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/system/compatibility.h>

#include <memory>

namespace campaigns {

class Campaign : public ugdk::action::Scene {
  public:
    static Campaign* CurrentCampaign();

    Campaign(const CampaignDescriptor&);
    ~Campaign();

    core::World* current_level() const { return current_level_; }
    ugdk::script::VirtualObj implementation() { return implementation_; }

    bool LoadLevel(const std::string& levelname);

    void Start();
    void End() override;

    void Focus() override;

    void InformSceneFinished();

  private:

    core::World* current_level_;
    CampaignDescriptor descriptor_;
    ugdk::script::VirtualObj implementation_;
    bool auto_started_;
};

} // namespace campaigns

#endif // HORUSEYE_GAME_CAMPAIGNS_CAMPAIGN_H_
