#include <ugdk/action.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/node.h>

#include "scrollingimagescene.h"

namespace scene {

#define BG  0
#define IMG 1

using namespace ugdk;
using ugdk::action::Task;
using ugdk::action::Scene;
using ugdk::graphic::Node;
using ugdk::math::Vector2D;

class ScrollingTask {
public:
    ScrollingTask(double time, Node* target, Scene* scene) : time_(time), target_(target), scene_(scene) {
        double delta_h = VIDEO_MANAGER()->video_size().y;
        if(target->drawable()) delta_h += target->drawable()->height();
        movement_.y = -delta_h / time_;

        Vector2D offset = target->geometry().offset() + Vector2D(0, VIDEO_MANAGER()->video_size().y);
        target->geometry().set_offset(offset);
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

ScrollingImageScene::ScrollingImageScene(ugdk::graphic::Drawable *background, ugdk::graphic::Drawable *image, double time) :
         ImageScene(background, image) {

    if(scene_layers_[IMG]) {
        AddTask(ScrollingTask(time, scene_layers_[IMG], this));
    }
}

}
