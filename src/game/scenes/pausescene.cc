#include "pausescene.h"
#include <ugdk/graphic/image.h>
#include <ugdk/action/sprite.h>
#include <ugdk/action/layer.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/base/engine.h>

namespace scene {

using namespace ugdk;

PauseScene::PauseScene () {
    image_ = new Image;
	image_->set_frame_size(VIDEO_MANAGER()->video_size());
    image_->set_color(0.5f, 0.5f, 0.5f);
    image_->set_alpha(0.5f);
    Layer *layer = new Layer;
    Sprite *sprite = new Sprite;
    sprite->Initialize(image_);
    AddLayer(layer);
    layer->AddSprite(sprite);
}

PauseScene::~PauseScene () {}

void PauseScene::End() {
    image_->Destroy();
    delete image_;
}


void PauseScene::Update (float delta_t) {
    InputManager *input = Engine::reference()->input_manager();
    if (input->KeyPressed(K_RETURN)) Finish();
}

}
