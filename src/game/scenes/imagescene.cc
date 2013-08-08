#include "imagescene.h"

#include <functional>

#include <ugdk/system/engine.h>
#include <ugdk/action.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/input/module.h>
#include "game/utils/levelmanager.h"
#include <ugdk/input/keys.h>

namespace scene {

using ugdk::action::Task;
using std::bind;
using namespace std::placeholders;

#define BG  0
#define IMG 1

static bool FinishImageSceneTask(ugdk::action::Scene* scene, double) {
    ugdk::input::Manager *input = ugdk::input::manager();
    if (input->KeyPressed(ugdk::input::K_RETURN) || input->KeyPressed(ugdk::input::K_ESCAPE) ||
        input->KeyPressed(ugdk::input::K_KP_ENTER) || input->MouseReleased(ugdk::input::M_BUTTON_LEFT))
        scene->Finish();
    return true;
}

ImageScene::ImageScene(ugdk::graphic::Drawable *background, ugdk::graphic::Drawable *image) {
    // Node [0], background image
    if (background) {
        scene_layers_[BG] = new ugdk::graphic::Node(background);
        scene_layers_[BG]->set_zindex(-1.0);
    }
    else scene_layers_[BG] = NULL;

    // Node [1], main image
    if (image) {
        scene_layers_[IMG] = new ugdk::graphic::Node(image);
        ugdk::math::Vector2D offset = (ugdk::graphic::manager()->video_size() - image->size())* 0.5;
        scene_layers_[IMG]->geometry().set_offset(offset);
    }
    else scene_layers_[IMG] = NULL;

    this->AddTask(bind(FinishImageSceneTask, this, _1));
    set_render_function([this](const ugdk::graphic::Geometry& geometry, const ugdk::graphic::VisualEffect& effect) {
        if(scene_layers_[BG])
            scene_layers_[BG]->Render(geometry, effect);
        if(scene_layers_[IMG])
            scene_layers_[IMG]->Render(geometry, effect);
    });
}

ImageScene::~ImageScene() {}

void ImageScene::End() {
    super::End();
    this->set_visible(false);
}

}
