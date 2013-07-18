#include "loading.h"

#include <ugdk/action.h>
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/drawable.h>

#include "game/utils/levelmanager.h"
#include "game/utils/imagefactory.h"

namespace scene {

using ugdk::math::Vector2D;
using ugdk::action::Task;
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
        utils::LevelManager::reference()->LoadNextLevel();

        return false;
    }

  private:
    bool first_frame_;
};

Loading::Loading() {
    Drawable* loading_image = ugdk::resource::GetLanguageWord("Loading")->CreateLabel();
    loading_image->set_hotspot(Drawable::BOTTOM_RIGHT);

    Vector2D position = ugdk::graphic::manager()->video_size() - Vector2D(10.0, 10.0);

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
