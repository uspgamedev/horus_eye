#include "campaigndisplay.h"

#include "frontend/nativebuilders.h"
#include "frontend/gameview/hud.h"
#include "frontend/gameview/lightrendering.h"
#include "frontend/gameview/objectgraphic.h"

#include "game/core/world.h"
#include "game/sprites/worldobject.h"
#include "game/components/graphic.h"
#include "game/map/giantfloor.h"
#include "game/map/room.h"
#include "game/renders/shape.h"
#include "communication/direct.h"

#include <functional>

#include <ugdk/system/engine.h>
#include <ugdk/action.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/ui/node.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/ui/drawable/texturedrectangle.h>
#include <ugdk/input/events.h>
#include <ugdk/debug/profiler.h>
#include <ugdk/graphic/opengl.h>
#include <ugdk/text/module.h>
#include <ugdk/text/textbox.h>
#include <pyramidworks/collision/collisionmanager.h>

namespace frontend {
namespace gameview {

using ugdk::system::Task;
using ugdk::input::KeyPressedEvent;
using ugdk::input::MouseButtonReleasedEvent;
using ugdk::input::Scancode;
using std::bind;
using namespace ugdk;
using namespace std::placeholders;

namespace {
    bool render_sprites = true;
    bool render_collision = false;
    bool render_visibility = false;
    bool render_profiler = false;
    std::shared_ptr<text::TextBox> profiler_text(nullptr);

    CampaignDisplay* g_current_ = nullptr;

    std::vector<ObjectGraphic> object_graphics_;

    void RenderSprites(const core::World& world, const LightRendering& light_rendering, graphic::Canvas& canvas) {
        using namespace ugdk::graphic;

        TextureUnit light_unit = manager()->ReserveTextureUnit(light_rendering.light_texture());
        TextureUnit texture_unit = manager()->ReserveTextureUnit();

        int shader_changes = 0;
        int texture_changes = 0;

        for (const map::Room* room : world.active_rooms()) {
            room->floor()->Draw(canvas, light_unit, light_rendering);
        }

        glEnable(GL_DEPTH_TEST);

        for (const auto& graphic : object_graphics_) {
            const auto& primitive = graphic.primitive();

            if (primitive.shader_program() != canvas.shader_program()) {
                canvas.ChangeShaderProgram(primitive.shader_program());
                canvas.SendUniform("drawable_texture", texture_unit);
                canvas.SendUniform("light_texture", light_unit);
                canvas.SendUniform("LEVEL_SIZE", world.size());
                canvas.SendUniform("LIGHT_TEXTURE_PRECISION", 2*light_rendering.light_precision());
                shader_changes++;
            }

            if (primitive.texture() != texture_unit.texture()) {
                texture_unit.BindTexture(primitive.texture());
                texture_changes++;
            }

            const Geometry& geo = canvas.current_geometry();
            glm::vec4 position_ogl = geo.AsMat4() * glm::vec4(graphic.final_position().x, graphic.final_position().y, 0.0, 0.0);
            glm::vec4 render_off_ogl = geo.AsMat4() * glm::vec4(graphic.render_offset().x, graphic.render_offset().y, 0.0, 0.0);
            Vector2D lightpos = (Vector2D(position_ogl.x, position_ogl.y) + geo.offset() - Vector2D(render_off_ogl.x, render_off_ogl.y))* 0.5 + Vector2D(0.5, 0.5);
            Vector2D lightUV = light_rendering.CalculateUV(graphic.world_position());
            canvas.SendUniform("objectDepth", static_cast<float>(lightpos.y));
            canvas.SendUniform("lightUV", lightUV);

            canvas.PushAndCompose(primitive.visual_effect());
            primitive.drawfunction()(primitive, canvas);
            canvas.PopVisualEffect();
        }

        //printf("Room '%s' rendered with %d shader changes and %d texture changes.\n", name_.c_str(), shader_changes, texture_changes);
        glDisable(GL_DEPTH_TEST);
    }

    void RenderRect(const ugdk::graphic::GLTexture* texture, graphic::Canvas& canvas) {
        canvas.PushAndCompose(graphic::Geometry(math::Vector2D(200, 50), math::Vector2D(0.25, 0.25)));
        ugdk::ui::TexturedRectangle rect(texture);
        rect.Draw(canvas);
        canvas.PopGeometry();
    }
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

        light_rendering_->UpdateBuffers();

        auto& shaders = ugdk::graphic::manager()->shaders();
        shaders.ChangeFlag(ugdk::graphic::Manager::Shaders::USE_LIGHT_BUFFER, true);
        canvas.ChangeShaderProgram(shaders.current_shader());
        canvas.PushAndCompose(world->camera());
        if (render_sprites)
            RenderSprites(*world, *light_rendering_, canvas);

        shaders.ChangeFlag(ugdk::graphic::Manager::Shaders::USE_LIGHT_BUFFER, false);
        canvas.ChangeShaderProgram(shaders.current_shader());
        
        if(render_collision)
            for(auto collobject : world->collision_manager()->active_objects())
                renders::DrawCollisionObject(collobject, canvas);

        if(render_visibility)
            for(auto collobject : world->visibility_manager()->active_objects())
                renders::DrawCollisionObject(collobject, canvas);
                
        canvas.PopGeometry();
        {
            ugdk::debug::ProfileSection section("Hud");
            hud_->node()->Render(canvas);
        }

        if (render_profiler)
            profiler_text->Draw(canvas);
    });

    AddTask([this](double dt) {
        campaign_->Update(dt);
        if (campaign_->current_level())
            campaign_->current_level()->Update(dt);
    });
    AddTask([this](double dt) {
        if (hud_)
            hud_->Update(dt);
    });

    event_handler().AddListener<ugdk::input::KeyPressedEvent>([this](const ugdk::input::KeyPressedEvent& key) {
        if (key.scancode == ugdk::input::Scancode::ESCAPE) {
            if (this == &ugdk::system::CurrentScene())
                ugdk::system::PushSceneFactory(frontend::nativebuilders::PauseScene);
        }
    });

    if (!profiler_text)
        profiler_text.reset(new text::TextBox(
        "Press F10 to fetch profiler data.",
        graphic::manager()->screen()->size().x,
        ugdk::text::manager()->current_font()));
}

CampaignDisplay::~CampaignDisplay() {
    g_current_ = nullptr;
}

void CampaignDisplay::Focus() {
    set_active(true);
    if (started_) return;
    started_ = true;
    campaign_->Start();
}

void CampaignDisplay::DeFocus() {
    set_active(false);
}

void CampaignDisplay::End() {
    communication::notify::ChangeMusic("");
}

void CampaignDisplay::LevelLoaded() {
    light_rendering_ = ugdk::MakeUnique<LightRendering>(campaign_->current_level());
    hud_ = ugdk::MakeUnique<Hud>(campaign_->current_level());
    campaign_->current_level()->AddTask(Task([this](double) {
        if (campaign_->current_level())
            if (auto hero = campaign_->current_level()->hero().lock())
                light_rendering_->set_focused_position(hero->world_position());
    }, 1.0));
}

} // namespace gameview
} // namespace frontend