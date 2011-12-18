#include "loading.h"
#include <ugdk/graphic/image.h>
#include <ugdk/action/sprite.h>
#include <ugdk/action/layer.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include "game/utils/levelmanager.h"
#include "game/utils/textloader.h"
#include "game/utils/imagefactory.h"

namespace scene {

using namespace ugdk;

Loading::Loading() {
    Drawable* loading_image = TEXT_LOADER()->GetImage("Loading");

    Sprite* text_sprite = new Sprite;
    text_sprite->Initialize(loading_image);

    Vector2D position = VIDEO_MANAGER()->video_size() - loading_image->render_size()
            - Vector2D(10.0f, 10.0f);
    text_sprite->set_position(position);

    layer_ = new Layer;
    layer_->AddSprite(text_sprite);
    layer_->set_visible(false);

    Engine::reference()->PushInterface(layer_);
    set_visible(false);
    has_been_drawn_ = false;

    this->StopsPreviousMusic(false);
}

Loading::~Loading() {
    utils::LevelManager::reference()->InformLoadingDeleted();
}

void Loading::Update(float delta_t) {
    set_visible(true);
    layer_->set_visible(true);
    Scene::Update(delta_t);
    if(has_been_drawn_) {
        //Finish();
        utils::ImageFactory factory;

        // Load projectiles
        factory.MagicMissileImage();
        factory.FireballImage();
        factory.LightningImage();
        factory.LightImage();

        // Load explosions.
        factory.ExplosionImage();
        factory.QuakeImage();
        utils::LevelManager::reference()->LoadNextLevel();
    }
    has_been_drawn_ = !has_been_drawn_;
}

void Loading::End() {
    Engine::reference()->RemoveInterface(layer_);
    delete layer_;
}

}
