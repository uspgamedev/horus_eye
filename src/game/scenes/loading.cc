#include <ugdk/graphic/image.h>
#include <ugdk/action/sprite.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/node.h>

#include "loading.h"

#include "game/utils/levelmanager.h"
#include "game/utils/textloader.h"
#include "game/utils/imagefactory.h"

namespace scene {

using namespace ugdk;

Loading::Loading() {
    Drawable* loading_image = TEXT_LOADER()->GetImage("Loading");

    Vector2D position = VIDEO_MANAGER()->video_size() - loading_image->render_size() - Vector2D(10.0f, 10.0f);

    loading_ = new Node(loading_image);
    loading_->modifier()->set_offset(position);

    Engine::reference()->PushInterface(loading_);
    set_visible(false);
    loading_->set_visible(false);
    has_been_drawn_ = false;

    this->StopsPreviousMusic(false);
}

Loading::~Loading() {
    utils::LevelManager::reference()->InformLoadingDeleted();
}

void Loading::Update(float delta_t) {
    super::Update(delta_t);
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

void Loading::Focus() {
    super::Focus();
    loading_->set_visible(true);
}

void Loading::DeFocus() {
    super::DeFocus();
    loading_->set_visible(false);
}

void Loading::End() {
    super::End();
    Engine::reference()->RemoveInterface(loading_);
    delete loading_;
}

}
