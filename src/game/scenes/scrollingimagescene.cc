#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/node.h>

#include "scrollingimagescene.h"

namespace scene {

#define BG  0
#define IMG 1

using namespace ugdk;

ScrollingImageScene::ScrollingImageScene(ugdk::graphic::Drawable *background, ugdk::graphic::Drawable *image, double time) :
         ImageScene(background, image) {
   time_ = time;
   double delta_h = VIDEO_MANAGER()->video_size().y;
   if(image != NULL) delta_h += image->height();
   if (time > 0) movement_ = Vector2D(0, -delta_h/time);

   Vector2D offset = scene_layers_[IMG]->modifier()->offset() + Vector2D(0, VIDEO_MANAGER()->video_size().y);
   if(scene_layers_[IMG]) scene_layers_[IMG]->modifier()->set_offset(offset);
}

void ScrollingImageScene::Update(double delta_t) {
    ImageScene::Update(delta_t);
    if(time_ > 0.0) {
        if (scene_layers_[IMG]) {
            Vector2D new_offset = scene_layers_[IMG]->modifier()->offset() + movement_*delta_t;
            scene_layers_[IMG]->modifier()->set_offset(new_offset);
        }
        time_ -= delta_t;
    }
    else Finish();
}

}
