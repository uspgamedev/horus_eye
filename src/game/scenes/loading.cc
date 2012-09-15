#include "loading.h"

#include <ugdk/action/task.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/node.h>
#include <externals/ugdk-videomanager.h>
#include <ugdk/graphic/drawable/text.h>


#include "game/utils/levelmanager.h"
#include "game/utils/imagefactory.h"

namespace scene {

using namespace ugdk;
using ugdk::action::Task;
using ugdk::base::ResourceManager;
using ugdk::graphic::Drawable;
using ugdk::graphic::Node;

class LoadTask : public Task {
  public:
    LoadTask() : first_frame_(true) {}
    ~LoadTask() {}

    void operator()(double dt) {
        if(first_frame_) {
            first_frame_ = false;
            return;
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
        finished_ = true;
    }

  private:
    bool first_frame_;
};

Loading::Loading() {
    Drawable* loading_image = ResourceManager::CreateTextFromLanguageTag("Loading");
    loading_image->set_hotspot(Drawable::BOTTOM_RIGHT);

    Vector2D position = VIDEO_MANAGER()->video_size() - Vector2D(10.0, 10.0);

    Node* loading = new ugdk::graphic::Node(loading_image);
    loading->modifier()->set_offset(position);

    interface_node()->AddChild(loading);
    interface_node()->modifier()->set_visible(false);

    this->StopsPreviousMusic(false);
}

Loading::~Loading() {
    utils::LevelManager::reference()->InformLoadingDeleted();
}

void Loading::Focus() {
    super::Focus();
    interface_node()->modifier()->set_visible(true);
    this->AddTask(new LoadTask);
}

void Loading::DeFocus() {
    super::DeFocus();
    interface_node()->modifier()->set_visible(false);
}
}
