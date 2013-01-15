#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable/text.h>

#include "loading.h"

#include "game/utils/levelmanager.h"
#include "game/utils/imagefactory.h"

namespace scene {

using namespace ugdk;

Loading::Loading() {
    ugdk::graphic::Drawable* loading_image = ugdk::base::ResourceManager::CreateTextFromLanguageTag("Loading");

    Vector2D position = VIDEO_MANAGER()->video_size() - loading_image->size() - Vector2D(10.0, 10.0);

    loading_ = new ugdk::graphic::Node(loading_image);
    loading_->modifier()->set_offset(position);

	interface_node()->AddChild(loading_);
    loading_->modifier()->set_visible(false);
    has_been_drawn_ = false;

    this->StopsPreviousMusic(false);
}

Loading::~Loading() {
    utils::LevelManager::reference()->InformLoadingDeleted();
}

void Loading::Update(double delta_t) {
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
    loading_->modifier()->set_visible(true);
}

void Loading::DeFocus() {
    super::DeFocus();
    loading_->modifier()->set_visible(false);
}
}
