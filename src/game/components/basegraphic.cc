#include "game/components/basegraphic.h"

#include <glm/glm.hpp>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
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
  : root_node_(new Node),
    node_(new Node),
    is_blinking_(false),
    layer_(scene::FOREGROUND_LAYER),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(0.0) { setup(); }

BaseGraphic::BaseGraphic(ugdk::graphic::Drawable* drawable)
  : root_node_(new Node),
    node_(new Node(drawable)),
    is_blinking_(false),
    layer_(scene::FOREGROUND_LAYER),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(0.0) { setup(); }

BaseGraphic::BaseGraphic(ugdk::graphic::Drawable* drawable, double light_radius)
  : root_node_(new Node),
    node_(new Node(drawable)),
    is_blinking_(false),
    layer_(scene::FOREGROUND_LAYER),
    blink_time_(new ugdk::time::TimeAccumulator(75)),
    blink_duration_(new ugdk::time::TimeAccumulator(0)),
    blink_(false),
    light_radius_(light_radius) { setup(); }

BaseGraphic::~BaseGraphic() {
    delete root_node_;
    delete blink_time_;
    delete blink_duration_;
}

void BaseGraphic::SetPosition(const ugdk::math::Vector2D& position) {
    Vector2D screen_position = core::FromWorldCoordinates(position);
    root_node_->geometry().set_offset(screen_position);
    root_node_->set_zindex(screen_position.y);
}

void BaseGraphic::set_render_offset(const ugdk::math::Vector2D& render_offset) {
    render_offset_ = render_offset;
    node_->geometry().set_offset(render_offset_);
}

void BaseGraphic::ChangeLightRadius(double radius) {
    light_radius_ = radius;
    
    if(light_radius_ > constants::GetDouble("LIGHT_RADIUS_THRESHOLD")) {
        if(root_node_->light() == NULL) {
            root_node_->set_light(new ugdk::graphic::Light);
            root_node_->light()->set_color(light_color_);
        }

        Vector2D dimension = core::ConvertLightRadius(light_radius_);
        root_node_->light()->set_dimension(dimension * LIGHT_COEFFICIENT);

    } else {
        if(root_node_->light()) {
            delete root_node_->light();
            root_node_->set_light(NULL);
        }
    }
}

void BaseGraphic::ChangeLightColor(const ugdk::Color& color) {
    light_color_ = color;
    if(root_node_->light())
        root_node_->light()->set_color(light_color_);
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

void BaseGraphic::InsertIntoLayers(ugdk::graphic::Node** layers) {
    layers[layer_]->AddChild(root_node_);
}

void BaseGraphic::RemoveFromLayers(ugdk::graphic::Node** layers) {
    layers[layer_]->RemoveChild(root_node_);
}
    
void BaseGraphic::ChangeDrawable(ugdk::graphic::Drawable* drawable) {
    node_->set_drawable(drawable);
    using namespace ugdk::graphic;
    if(drawable)
        drawable->set_draw_setup_function([](const Drawable*, const Geometry& geo, const VisualEffect&) -> void {
            glm::vec4 lightpos = (geo.AsMat4() * glm::vec4(0.0, 0.0, 0.0, 1.0)) * 0.5 + glm::vec4(0.5, 0.5, 0.0, 0.0);
            ugdk::graphic::opengl::ShaderProgram::Use shader(get_horus_light_shader());
            shader.SendUniform("lightUV", lightpos.x, lightpos.y);
        });
}
    
void BaseGraphic::setup() {
    root_node_->AddChild(node_);
    if(light_radius_ > 0)
        ChangeLightRadius(light_radius_);
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
