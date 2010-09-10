#include "loading.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/engine.h"
#include "../../framework/textmanager.h"
#include "../utils/levelmanager.h"

namespace scene {

using namespace framework;

Loading::Loading() {
    TEXT_MANAGER()->setFont("data/font/Filmcryptic.ttf", 50, NULL);
    loading_image_ = TEXT_MANAGER()->LoadLine("Loading...");

    Sprite* text_sprite = new Sprite;
    text_sprite->Initialize(loading_image_);

    Vector2D position = VIDEO_MANAGER()->video_size() - loading_image_->frame_size()
            - Vector2D(10.0f, 10.0f);
    text_sprite->set_position(position);

    Layer * layer = new Layer;
    layer->AddSprite(text_sprite);
    AddLayer(layer);
    set_visible(false);
    has_been_drawn_ = false;
}
Loading::~Loading() {
    delete loading_image_;
}

void Loading::Update(float delta_t) {
    set_visible(true);
    Scene::Update(delta_t);
    if(has_been_drawn_) {
        Finish();
        utils::LevelManager::reference()->StartGame();
    }
    has_been_drawn_ = true;
}

}
