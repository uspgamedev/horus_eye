#include "campaigndisplay.h"

#include "frontend/nativebuilders.h"
#include "game/core/world.h"
#include "game/core/lightrendering.h"
#include "game/map/room.h"

#include <functional>

#include <ugdk/system/engine.h>
#include <ugdk/action.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/ui/node.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/input/events.h>

namespace frontend {
namespace scenes {

using ugdk::system::Task;
using ugdk::input::KeyPressedEvent;
using ugdk::input::MouseButtonReleasedEvent;
using ugdk::input::Scancode;
using std::bind;
using namespace ugdk;
using namespace std::placeholders;

namespace {
    bool render_sprites = true;
    CampaignDisplay* g_current_ = nullptr;
}

CampaignDisplay* CampaignDisplay::Current() {
    return g_current_;
}

CampaignDisplay::CampaignDisplay(campaigns::Campaign* campaign)
    : campaign_(campaign)
    , started_(false)
{
    g_current_ = this;
    set_render_function([this](graphic::Canvas& canvas) {
        auto world = campaign_->current_level();
        if (!world)
            return;

        light_rendering_->Render(canvas);

        auto& shaders = ugdk::graphic::manager()->shaders();
        shaders.ChangeFlag(ugdk::graphic::Manager::Shaders::USE_LIGHT_BUFFER, true);
        canvas.ChangeShaderProgram(shaders.current_shader());
        canvas.PushAndCompose(world->camera());
        if(render_sprites)
            for(const map::Room* room : world->active_rooms())
                room->Render(canvas);

        shaders.ChangeFlag(ugdk::graphic::Manager::Shaders::USE_LIGHT_BUFFER, false);
        canvas.ChangeShaderProgram(shaders.current_shader());

        /*
        if(render_collision)
            for(auto collobject : collision_manager_.active_objects())
                renders::DrawCollisionObject(collobject, canvas);
        if(render_visibility)
            for(auto collobject : visibility_manager_.active_objects())
                renders::DrawCollisionObject(collobject, canvas);
                */

        canvas.PopGeometry();
        {
            //ugdk::debug::ProfileSection section("Hud");
            //world->hud()->node()->Render(canvas);
        }

        //if (render_profiler)
        //    profiler_text->Draw(canvas);
    });

    AddTask([this](double dt) {
        campaign_->Update(dt);
        if (campaign_->current_level())
            campaign_->current_level()->Update(dt);
    });

    event_handler().AddListener<ugdk::input::KeyPressedEvent>([this](const ugdk::input::KeyPressedEvent& key) {
        if (key.scancode == ugdk::input::Scancode::ESCAPE) {
            if (this == &ugdk::system::CurrentScene())
                ugdk::system::PushSceneFactory(frontend::nativebuilders::PauseScene);
        }
    });
}

CampaignDisplay::~CampaignDisplay() {
    g_current_ = nullptr;
}

void CampaignDisplay::Focus() {
    if (started_) return;
    started_ = true;
    campaign_->Start();
}

void CampaignDisplay::LevelLoaded() {
    light_rendering_ = ugdk::MakeUnique<core::LightRendering>(campaign_->current_level());
    campaign_->current_level()->set_light_rendering(light_rendering_.get());
}

} // namespace scenes
} // namespace frontend