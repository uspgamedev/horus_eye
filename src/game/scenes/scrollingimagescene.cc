#include "scrollingimagescene.h"
#include "../../framework/engine.h"
namespace scene {

#define BG  0
#define IMG 1

using namespace framework;

ScrollingImageScene::ScrollingImageScene(Image *background, Image *image, float time) :
         ImageScene(background, image) {
   time_ = time;
   float delta_h = VIDEO_MANAGER()->video_size().y;
   if(image != NULL)
	   delta_h += image->height();
   Vector2D offset(0, -VIDEO_MANAGER()->video_size().y);
   if(scene_layers_[IMG])
       scene_layers_[IMG]->set_offset(offset);
   if (time > 0)
       movement_ = Vector2D(0, delta_h/time);
}

void ScrollingImageScene::Update(float delta_t) {
    ImageScene::Update(delta_t);
    if(time_ > 0.0f) {
        if (scene_layers_[IMG]) {
            Layer *img_layer = scene_layers_[IMG];
            Vector2D new_offset = img_layer->offset() + movement_*delta_t;
            img_layer->set_offset(new_offset);
        }
        time_ -= delta_t;
    }
    else Finish();
}

}
