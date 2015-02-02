#include "game/components/graphic.h"

#include <glm/glm.hpp>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/sprite.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/resource/module.h>

#include "game/core/coordinates.h"
#include "game/components/animator.h"
#include "game/sprites/worldobject.h"
#include "game/initializer.h"

#include "game/map/specialwall.h"

namespace component {

using ugdk::math::Vector2D;
using ugdk::graphic::Primitive;

namespace {

    void SpecialWallCreate(Primitive& p, const std::string& frame) {
        map::PreparePrimitiveSpecialWall(p, ugdk::resource::GetTextureAtlasFromTag("wall"), frame);
    }

    std::vector<void(*)(Primitive&, const std::string&)> CreateFunctions = {
        SpecialWallCreate
    };
}

Graphic::Graphic()
: primitive_(nullptr, nullptr)
, layer_(core::FOREGROUND_LAYER)
{}

Graphic::~Graphic() {}
    
void Graphic::UpdateFinalPosition() {
    final_position_ = core::FromWorldCoordinates(world_position_) + render_offset_;
    if (auto controlller = primitive_.controller().get()) {
        controlller->ChangePosition(final_position_);
    }
}
    
void Graphic::SetPosition(const ugdk::math::Vector2D& position) {
    world_position_ = position;
    UpdateFinalPosition();
}

void Graphic::set_render_offset(const ugdk::math::Vector2D& render_offset) {
    render_offset_ = render_offset;
    UpdateFinalPosition();
}
    
double Graphic::alpha() const {
    return visual_effect_.color().a;
}

void Graphic::ChangeAlpha(double alpha) {
    ugdk::Color color = visual_effect_.color();
    color.a = alpha;
    visual_effect_.set_color(color);
}

bool Graphic::visible() const {
    return visual_effect_.visible();
}

void Graphic::set_visible(bool visible) {
    visual_effect_.set_visible(visible);
}

void Graphic::Update(double dt) {}

void Graphic::OnAdd(sprite::WObjRawPtr wobj) {
    SetPosition(wobj->world_position());
    if (auto animator = wobj->animator())
        animator->Configure(this);
}

void Graphic::OnObjectRemoved() {
    set_visible(false);
}
    
void Graphic::ChangeToFrame(const std::string& frame_name) {
    auto controller = dynamic_cast<ugdk::graphic::PrimitiveControllerSprite*>(primitive_.controller().get());
    if (controller)
        controller->ChangeToAtlasFrame(frame_name);
}

void Graphic::ChangeToFrame(std::size_t frame_number) {
    auto controller = dynamic_cast<ugdk::graphic::PrimitiveControllerSprite*>(primitive_.controller().get());
    if (controller)
        controller->ChangeToAtlasFrame(frame_number);
}
    
namespace {
    void SetDefaultShader(Graphic* g) {
        if (!g->primitive().shader_program())
            g->primitive().set_shader_program(get_horus_light_shader());
    }
}

Graphic* Graphic::Create(CreateTypes type, const std::string& arg) {
    Graphic* g = new Graphic;
    CreateFunctions[static_cast<int>(type)](g->primitive(), arg);
    SetDefaultShader(g);
    return g;
}

Graphic* Graphic::CreateWithSpritesheet(const std::string& spritesheet_name) {
    Graphic* g = new Graphic;
    ugdk::graphic::PrimitiveSetup::Sprite::Prepare(g->primitive(), ugdk::resource::GetTextureAtlasFromTag(spritesheet_name));
    g->ChangeToFrame(0);
    SetDefaultShader(g);
    return g;
}

Graphic* Graphic::CreateWithSingleFrame(const std::string& spritesheet_name, const std::string& frame_name) {
    Graphic* g = Graphic::CreateWithSpritesheet(spritesheet_name);
    g->ChangeToFrame(frame_name);
    return g;
}
    
}  // namespace component
