#include "loading.h"
#include "ugdk/image.h"
#include "ugdk/sprite.h"
#include "ugdk/engine.h"
#include "ugdk/videomanager.h"
#include "../utils/levelmanager.h"
#include "../utils/textloader.h"

namespace scene {

using namespace ugdk;

Loading::Loading() {
    Drawable* loading_image = TEXT_LOADER()->GetImage("Loading");

    Sprite* text_sprite = new Sprite;
    text_sprite->Initialize(loading_image);

    Vector2D position = VIDEO_MANAGER()->video_size() - loading_image->render_size()
            - Vector2D(10.0f, 10.0f);
    text_sprite->set_position(position);

    Layer * layer = new Layer;
    layer->AddSprite(text_sprite);

    AddLayer(layer);
    set_visible(false);
    has_been_drawn_ = false;
}

Loading::~Loading() {
    utils::LevelManager::reference()->InformLoadingDeleted();
}

void Loading::Update(float delta_t) {
    set_visible(true);
    Scene::Update(delta_t);
    if(has_been_drawn_) {
        //Finish();
        utils::LevelManager::reference()->LoadNextLevel();
    }
    has_been_drawn_ = !has_been_drawn_;
}

}
