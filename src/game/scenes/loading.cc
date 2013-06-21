#include "loading.h"

#include <ugdk/action.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable.h>

#include "game/utils/levelmanager.h"
#include "game/utils/imagefactory.h"

namespace scene {

using ugdk::math::Vector2D;
using ugdk::action::Task;
using ugdk::base::ResourceManager;
using ugdk::graphic::Drawable;
using ugdk::graphic::Node;

class LoadTask {
  public:
    LoadTask() : first_frame_(true) {}
    ~LoadTask() {}

    bool operator()(double dt) {
        if(first_frame_) {
            first_frame_ = false;
            return true;
        }
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

        return false;
    }

  private:
    bool first_frame_;
};

Loading::Loading() {
    Drawable* loading_image = ResourceManager::GetLanguageWord("Loading")->CreateLabel();
    loading_image->set_hotspot(Drawable::BOTTOM_RIGHT);

    Vector2D position = VIDEO_MANAGER()->video_size() - Vector2D(10.0, 10.0);

    Node* loading = new ugdk::graphic::Node(loading_image);
    loading->geometry().set_offset(position);

    interface_node()->AddChild(loading);
    interface_node()->effect().set_visible(false);

    this->StopsPreviousMusic(false);
}

Loading::~Loading() {
    utils::LevelManager::reference()->InformLoadingDeleted();
}

void Loading::Focus() {
    super::Focus();
    interface_node()->effect().set_visible(true);
    this->AddTask(LoadTask());
}

void Loading::DeFocus() {
    super::DeFocus();
    interface_node()->effect().set_visible(false);
}
}
