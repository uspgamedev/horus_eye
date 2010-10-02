#include "loading.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../utils/levelmanager.h"
#include "../utils/textloader.h"

namespace scene {

using namespace framework;

Loading::Loading() {
    Image* loading_image = TEXT_LOADER()->GetImage("Loading");

    Sprite* text_sprite = new Sprite;
    text_sprite->Initialize(loading_image);

    Vector2D position = VIDEO_MANAGER()->video_size() - loading_image->frame_size()
            - Vector2D(10.0f, 10.0f);
    text_sprite->set_position(position);

    Layer * layer = new Layer;
    layer->AddSprite(text_sprite);

    AddLayer(layer);
    set_visible(false);
    has_been_drawn_ = false;
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
