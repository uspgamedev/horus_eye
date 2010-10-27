#include "pausescene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/layer.h"
#include "../../framework/inputmanager.h"
#include "../../framework/engine.h"

namespace scene {

using namespace framework;

PauseScene::PauseScene () {
    image_ = new Image;
    image_->Create(VIDEO_MANAGER()->video_size(), SDL_HWSURFACE);
    image_->Clear(0x808080);
    image_->SetAlpha(128);
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
