#include "imagescene.h"

#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)

#include <ugdk/base/engine.h>
#include <ugdk/action/generictask.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/input/inputmanager.h>
#include "game/utils/levelmanager.h"
#include <ugdk/input/keys.h>

namespace scene {

using ugdk::action::GenericTask;
using std::tr1::bind;
using namespace std::tr1::placeholders;

#define BG  0
#define IMG 1

static bool FinishImageSceneTask(ugdk::action::Scene* scene, double) {
    ugdk::input::InputManager *input = INPUT_MANAGER();
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
        interface_node()->AddChild(scene_layers_[BG]);
    }
    else scene_layers_[BG] = NULL;

    // Node [1], main image
    if (image) {
        scene_layers_[IMG] = new ugdk::graphic::Node(image);

        ugdk::math::Vector2D offset = (VIDEO_MANAGER()->video_size() - image->size())* 0.5;
        scene_layers_[IMG]->geometry().set_offset(offset);

        interface_node()->AddChild(scene_layers_[IMG]);
    }
    else scene_layers_[IMG] = NULL;

    this->AddTask(new GenericTask(bind(FinishImageSceneTask, this, _1)));
}

ImageScene::~ImageScene() {}

void ImageScene::End() {
    super::End();
    interface_node()->effect().set_visible(false);
}

}
