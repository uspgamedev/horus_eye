
#include "communication/direct.h"

#include "frontend/gameview/campaigndisplay.h"
#include "frontend/settings.h"

#include <ugdk/audio/module.h>

namespace communication {

namespace notify {
    void CampaignLevelLoaded() {
        frontend::gameview::CampaignDisplay::Current()->LevelLoaded();
    }

    void CampaignFailedToLoadLevel(const std::string&) {

    }

    void PlaySound(const std::string& sfx) {
        if (frontend::Settings::reference()->sound_effects())
            ugdk::audio::manager()->LoadSample(sfx)->Play();
    }

    void ChangeMusic(const std::string& sfx) {
        auto current_music = ugdk::audio::manager()->CurrentMusic();
        if (sfx.empty()) {
            if (current_music) {
                current_music->Stop();
            }
        } else if (frontend::Settings::reference()->background_music()) {
            auto new_music = ugdk::audio::manager()->LoadMusic(sfx);
            if (new_music != current_music)
                new_music->PlayForever();
        }
    }
}

} // namespace frontend