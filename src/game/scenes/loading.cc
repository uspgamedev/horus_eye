#include "loading.h"

#include <functional>
#include <ugdk/action.h>
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/drawable.h>

#include "game/utils/levelmanager.h"
#include "game/utils/imagefactory.h"

namespace scene {

using ugdk::math::Vector2D;
using ugdk::system::Task;
using ugdk::graphic::Drawable;
using ugdk::graphic::Node;
using namespace std::placeholders;

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

    Vector2D position = ugdk::graphic::manager()->canvas()->size() - Vector2D(10.0, 10.0);

    loading_ = new ugdk::graphic::Node(loading_image);
    loading_->geometry().set_offset(position);

    set_visible(false);
    set_render_function(std::bind(std::mem_fn(&ugdk::graphic::Node::Render), loading_, _1));

    this->StopsPreviousMusic(false);
}

Loading::~Loading() {
    delete loading_;
    utils::LevelManager::reference()->InformLoadingDeleted();
}

void Loading::Focus() {
    super::Focus();
    this->set_visible(true);
    this->AddTask(LoadTask());
}

void Loading::DeFocus() {
    super::DeFocus();
    this->set_visible(false);
}
}
