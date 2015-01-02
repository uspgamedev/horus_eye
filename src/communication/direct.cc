
#include "communication/direct.h"

#include "frontend/scenes/campaigndisplay.h"

namespace communication {

namespace notify {
    void CampaignLevelLoaded() {
        frontend::scenes::CampaignDisplay::Current()->LevelLoaded();
    }

    void CampaignFailedToLoadLevel(const std::string&) {

    }
}

} // namespace frontend