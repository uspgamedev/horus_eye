
#include "frontend/debugtools.h"

#include "frontend/scenes/console.h"
#include "frontend/gameview/campaigndisplay.h"
#include "frontend/gameview/lightrendering.h"

#include "game/campaigns/campaign.h"
#include "game/sprites/worldobject.h"
#include "game/core/world.h"
#include "game/core/coordinates.h"
#include "game/components/caster.h"
#include "game/components/damageable.h"

#include <ugdk/input/events.h>
#include <ugdk/input/module.h>
#include <ugdk/system/compatibility.h>

namespace frontend {

namespace {

using namespace ugdk;

class DebugTools :
    public ugdk::system::Listener < ugdk::input::KeyPressedEvent >
{
    public:
    void Handle(const ugdk::input::KeyPressedEvent& ev) override {
        auto campaign_display = gameview::CampaignDisplay::Current();

        auto current_campaign = campaigns::Campaign::CurrentCampaign();
        core::World* world = current_campaign ? current_campaign->current_level() : nullptr;
        sprite::WObjPtr hero = world ? world->hero().lock() : nullptr;

        if (hero) {
            if (ev.keycode == input::Keycode::h) {
                if (ev.modifiers & input::Keymod::SHIFT)
                    hero->caster()->mana_blocks().Fill();
                hero->damageable()->life().Fill();
                hero->caster()->mana().Fill();
            }
            if (ev.keycode == input::Keycode::t)
                hero->set_world_position(core::FromScreenCoordinates(world, input::manager()->mouse().position()));
            if (ev.keycode == input::Keycode::m)
                hero->damageable()->TakeDamage(1000.0);
        }

        if (world) {
            ugdk::math::Geometry& modifier = const_cast<ugdk::math::Geometry&>(world->camera());
            {
                math::Vector2D scale(1.0);
                if (ev.keycode == input::Keycode::NUMPAD_MULTIPLY)
                    scale = scale * 1.4 / 1.0;
                if (ev.keycode == input::Keycode::NUMPAD_DIVIDE)
                    scale = scale * 1.0 / 1.4;
                modifier *= math::Geometry(math::Vector2D(), scale);
            }
        }

        if (campaign_display && campaign_display->light_rendering()) {
            if (ev.keycode == input::Keycode::l)
                campaign_display->light_rendering()->ToggleLightsystem();

            if (ev.keycode == input::Keycode::k)
                campaign_display->light_rendering()->ToggleShadowcasting();
        }

        /*
        if (ev.keycode == input::Keycode::i)
            render_sprites = !render_sprites;

        if (ev.keycode == input::Keycode::u)
            render_collision = !render_collision;

        if (ev.keycode == input::Keycode::j)
            render_visibility = !render_visibility;

        if (ev.scancode == input::Scancode::F9)
            render_profiler = !render_profiler;           

        if (ev.scancode == input::Scancode::F10 || ev.scancode == input::Scancode::MENU) {
            const auto& datalist = ugdk::system::profile_data_list();
            if (!datalist.empty()) {
                std::stringstream msg;
                renders::SectionDataToString(msg, "", datalist.back());
                profiler_text->ChangeMessage(msg.str());
            }
            if (ev.scancode == input::Scancode::MENU)
                render_profiler = true;
        }
        */

        if (ev.scancode == input::Scancode::GRAVE && !frontend::scenes::Console::HasConsoleActive()) {
            ugdk::system::PushSceneFactory(&ugdk::MakeUnique<frontend::scenes::Console>);
        }
    }
};

    std::unique_ptr<DebugTools> debug_tools_listener_;
}

ugdk::system::IListener* DebugToolsListener() {
    if (!debug_tools_listener_)
        debug_tools_listener_.reset(new DebugTools);
    return debug_tools_listener_.get();
}

} // namespace frontend