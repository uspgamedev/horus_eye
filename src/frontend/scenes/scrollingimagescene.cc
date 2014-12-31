
#include "scrollingimagescene.h"

#include <ugdk/action.h>
#include <ugdk/system/engine.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/module.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/ui/node.h>

namespace frontend {
namespace scenes {

#define BG  0
#define IMG 1

using namespace ugdk;
using ugdk::system::Task;
using ugdk::action::Scene;
using ugdk::ui::Node;
using ugdk::math::Vector2D;

class ScrollingTask {
  public:
    ScrollingTask(double time, Node* target, Scene* scene) : time_(time), target_(target), scene_(scene) {

        auto d = target->drawable();

        Vector2D hotspot = d->hotspot();
        hotspot.y = 0.0;
        d->set_hotspot(hotspot);

        Vector2D offset = target->geometry().offset();
        offset.y = graphic::manager()->screen()->size().y;
        target->geometry().set_offset(offset);

        double delta_h = offset.y + d->height();
        movement_.y = -delta_h / time_;
    }

    bool operator()(double dt) {
        Vector2D new_offset = target_->geometry().offset() + movement_*dt;
        target_->geometry().set_offset(new_offset);

        time_ -= dt;
        bool finished = (time_ < 0.0);
        if(finished && scene_) scene_->Finish();
        return !finished;
    }

  private:
    double time_;
    Node* target_;
    Scene* scene_;
    Vector2D movement_;
};

ScrollingImageScene::ScrollingImageScene(std::unique_ptr<ugdk::ui::Drawable>&& background, std::unique_ptr<ugdk::ui::Drawable>&& image, double time)
:  ImageScene(std::move(background), std::move(image))
{
    if(scene_layers_[IMG] && scene_layers_[IMG]->drawable()) {
        AddTask(ScrollingTask(time, scene_layers_[IMG].get(), this));
    }
}

} // namespace scenes
} // namespace frontend
