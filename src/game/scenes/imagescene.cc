#include "imagescene.h"
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/input/inputmanager.h>
#include "game/utils/levelmanager.h"
#include <ugdk/input/keys.h>

namespace scene {

#define BG  0
#define IMG 1

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

        ugdk::Vector2D offset = (VIDEO_MANAGER()->video_size() - image->size())* 0.5;
        scene_layers_[IMG]->modifier()->set_offset(offset);

        interface_node()->AddChild(scene_layers_[IMG]);
    }
    else scene_layers_[IMG] = NULL;
}

ImageScene::~ImageScene() {}

void ImageScene::End() {
    super::End();
	interface_node()->modifier()->set_visible(false);
}

void ImageScene::Update(double delta_t) {
    super::Update(delta_t);
    ugdk::input::InputManager *input = INPUT_MANAGER();
    if (input->KeyPressed(ugdk::input::K_RETURN) || input->KeyPressed(ugdk::input::K_ESCAPE) ||
        input->KeyPressed(ugdk::input::K_KP_ENTER) || input->MouseUp(ugdk::input::M_BUTTON_LEFT))
        Finish();
}

}
