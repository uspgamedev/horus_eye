#include "objectgraphic.h"

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

namespace frontend {
namespace gameview {

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

ObjectGraphic::ObjectGraphic()
: primitive_(nullptr, nullptr)
, layer_(core::FOREGROUND_LAYER)
{}

ObjectGraphic::~ObjectGraphic() {}
    
void ObjectGraphic::UpdateFinalPosition() {
    final_position_ = core::FromWorldCoordinates(world_position_) + render_offset_;
    if (auto controlller = primitive_.controller().get()) {
        controlller->ChangePosition(final_position_);
    }
}
    
void ObjectGraphic::SetPosition(const ugdk::math::Vector2D& position) {
    world_position_ = position;
    UpdateFinalPosition();
}

void ObjectGraphic::set_render_offset(const ugdk::math::Vector2D& render_offset) {
    render_offset_ = render_offset;
    UpdateFinalPosition();
}
    
double ObjectGraphic::alpha() const {
    return visual_effect_.color().a;
}

void ObjectGraphic::ChangeAlpha(double alpha) {
    ugdk::Color color = visual_effect_.color();
    color.a = alpha;
    visual_effect_.set_color(color);
}

bool ObjectGraphic::visible() const {
    return visual_effect_.visible();
}

void ObjectGraphic::set_visible(bool visible) {
    visual_effect_.set_visible(visible);
}
    
void ObjectGraphic::ChangeToFrame(const std::string& frame_name) {
    auto controller = dynamic_cast<ugdk::graphic::PrimitiveControllerSprite*>(primitive_.controller().get());
    if (controller)
        controller->ChangeToAtlasFrame(frame_name);
}

void ObjectGraphic::ChangeToFrame(std::size_t frame_number) {
    auto controller = dynamic_cast<ugdk::graphic::PrimitiveControllerSprite*>(primitive_.controller().get());
    if (controller)
        controller->ChangeToAtlasFrame(frame_number);
}

void ObjectGraphic::ChangeToAnimationFrame(const ugdk::graphic::SpriteAnimationFrame& frame) {
    auto controller = dynamic_cast<ugdk::graphic::PrimitiveControllerSprite*>(primitive_.controller().get());
    if (controller)
        controller->ChangeToAnimationFrame(frame);
}
    
namespace {
    void SetDefaultShader(Primitive& p) {
        if (!p.shader_program())
            p.set_shader_program(get_horus_light_shader());
    }
}

ObjectGraphic* ObjectGraphic::Create(CreateTypes type, const std::string& arg) {
    ObjectGraphic* g = new ObjectGraphic;
    CreateFunctions[static_cast<int>(type)](g->primitive_, arg);
    SetDefaultShader(g->primitive_);
    return g;
}

ObjectGraphic* ObjectGraphic::CreateWithSpritesheet(const std::string& spritesheet_name) {
    ObjectGraphic* g = new ObjectGraphic;
    ugdk::graphic::PrimitiveSetup::Sprite::Prepare(g->primitive_, ugdk::resource::GetTextureAtlasFromTag(spritesheet_name));
    g->ChangeToFrame(0);
    SetDefaultShader(g->primitive_);
    return g;
}

ObjectGraphic* ObjectGraphic::CreateWithSingleFrame(const std::string& spritesheet_name, const std::string& frame_name) {
    ObjectGraphic* g = ObjectGraphic::CreateWithSpritesheet(spritesheet_name);
    g->ChangeToFrame(frame_name);
    return g;
}
    
} // namespace gameview
} // namespace frontend
