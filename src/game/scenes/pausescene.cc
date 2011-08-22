#include "pausescene.h"
#include "ugdk/image.h"
#include "ugdk/sprite.h"
#include "ugdk/layer.h"
#include "ugdk/inputmanager.h"
#include "ugdk/engine.h"

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
