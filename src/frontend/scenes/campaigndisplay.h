#ifndef HORUSEYE_FRONTEND_SCENES_CAMPAIGNDISPLAY_H_
#define HORUSEYE_FRONTEND_SCENES_CAMPAIGNDISPLAY_H_

#include <ugdk/action/scene.h>

#include "game/campaigns/campaign.h"

#include <memory>

namespace frontend {
namespace scenes {

class CampaignDisplay : public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    CampaignDisplay(campaigns::Campaign*);
    ~CampaignDisplay();
    static CampaignDisplay* Current();

    void Focus() override;

    void LevelLoaded();

    core::LightRendering* light_rendering() const { return light_rendering_.get(); }

  private:
    std::unique_ptr<campaigns::Campaign> campaign_;
    std::unique_ptr<core::LightRendering> light_rendering_;
    bool started_;
};

} // namespace scenes
} // namespace frontend

#endif // HORUSEYE_FRONTEND_SCENES_CAMPAIGNDISPLAY_H_
