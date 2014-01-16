#include "game/components/basegraphic.h"

#include <glm/glm.hpp>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/time/timeaccumulator.h>
#include <ugdk/math/vector2D.h>

#include "game/components/orders.h"
#include "game/constants.h"
#include "game/core/coordinates.h"
#include "game/initializer.h"
#include "game/scenes/world.h"

#define LIGHT_COEFFICIENT 0.75

namespace component {

using ugdk::math::Vector2D;
using ugdk::graphic::Node;
using ugdk::graphic::Drawable;

BaseGraphic::BaseGraphic()
    :   root_node_(new Node)
    ,   node_(new Node)
    ,   is_blinking_(false)
    ,   layer_(scene::FOREGROUND_LAYER)
    ,   blink_time_(new ugdk::time::TimeAccumulator(75))
    ,   blink_duration_(new ugdk::time::TimeAccumulator(0))
    ,   blink_(false)
{ setup(); }

BaseGraphic::BaseGraphic(ugdk::graphic::Drawable* drawable)
    :   root_node_(new Node)
    ,   node_(new Node(drawable))
    ,   is_blinking_(false)
    ,   layer_(scene::FOREGROUND_LAYER)
    ,   blink_time_(new ugdk::time::TimeAccumulator(75))
    ,   blink_duration_(new ugdk::time::TimeAccumulator(0))
    ,   blink_(false)
{ setup(); }

BaseGraphic::~BaseGraphic() {
    delete root_node_;
    delete blink_time_;
    delete blink_duration_;
}

void BaseGraphic::SetPosition(const ugdk::math::Vector2D& position) {
    Vector2D screen_position = core::FromWorldCoordinates(position);
    root_node_->geometry().set_offset(screen_position);
}

void BaseGraphic::set_render_offset(const ugdk::math::Vector2D& render_offset) {
    render_offset_ = render_offset;
    node_->geometry().set_offset(render_offset_);
}

double BaseGraphic::alpha() const {
    return node_->effect().color().a;
}

void BaseGraphic::ChangeAlpha(double alpha) {
    ugdk::Color color = node_->effect().color();
    color.a = alpha;
    node_->effect().set_color(color);
}

void BaseGraphic::set_visible(bool visible) {
    root_node_->effect().set_visible(visible);
}

void BaseGraphic::Update(double dt) {
    adjustBlink();
}

void BaseGraphic::StartBlinking(int duration) {
    is_blinking_ = true;
    blink_time_->Restart();
    if(duration <= 0)
        blink_duration_->Pause();
    else
        blink_duration_->Restart(duration);
}

void BaseGraphic::StopBlinking() {
    is_blinking_ = false;
    node_->effect().set_visible(true);
}
    
void BaseGraphic::Render(ugdk::graphic::Canvas& canvas) const {
    root_node_->Render(canvas);
}

void BaseGraphic::ChangeDrawable(ugdk::graphic::Drawable* drawable) {
    node_->set_drawable(drawable);
    using namespace ugdk::graphic;
    if(drawable)
        drawable->set_draw_setup_function([this](const Drawable*, Canvas& canvas) -> void {
            const Geometry& geo = canvas.current_geometry();
            glm::vec4 render_ogl = geo.AsMat4() * glm::vec4(render_offset_.x, render_offset_.y, 0.0, 0.0);
            Vector2D lightpos = (geo.offset() - Vector2D(render_ogl.x, render_ogl.y))* 0.5 + Vector2D(0.5, 0.5);

            ugdk::graphic::opengl::ShaderProgram::Use shader(get_horus_light_shader());
            shader.SendUniform("lightUV", lightpos.x, lightpos.y);
        });
}
    
void BaseGraphic::setup() {
    root_node_->AddChild(node_);
    ChangeDrawable(node_->drawable());
}

void BaseGraphic::adjustBlink() {
    if(is_blinking_ && !blink_duration_->IsPaused() && blink_duration_->Expired())
        StopBlinking();
    if (is_blinking_ && blink_time_->Expired()) {
        blink_ = !blink_;
        node_->effect().set_visible(blink_);
        blink_time_->Restart();
    }
}

}  // namespace component
