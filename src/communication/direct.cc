
#include "communication/direct.h"

#include "frontend/scenes/campaigndisplay.h"
#include "game/utils/settings.h"

#include <ugdk/audio/module.h>

namespace communication {

namespace notify {
    void CampaignLevelLoaded() {
        frontend::scenes::CampaignDisplay::Current()->LevelLoaded();
    }

    void CampaignFailedToLoadLevel(const std::string&) {

    }

    void PlaySound(const std::string& sfx) {
        if (utils::Settings::reference()->sound_effects())
            ugdk::audio::manager()->LoadSample(sfx)->Play();
    }
}

} // namespace frontend