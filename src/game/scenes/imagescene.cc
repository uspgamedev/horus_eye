#include "imagescene.h"
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/input/inputmanager.h>
#include "game/utils/levelmanager.h"
#include <ugdk/input/keys.h>

namespace scene {

#define BG  0
#define IMG 1

ImageScene::ImageScene(ugdk::graphic::Drawable *background, ugdk::graphic::Drawable *image) 
    :   interface_node_(new ugdk::graphic::Node) {


    // Node [0], background image
    if (background) {
        scene_layers_[BG] = new ugdk::graphic::Node(background);
        scene_layers_[BG]->set_zindex(-1.0f);
        interface_node_->AddChild(scene_layers_[BG]);
    }
    else scene_layers_[BG] = NULL;

    // Node [1], main image
    if (image) {
        scene_layers_[IMG] = new ugdk::graphic::Node(image);

        ugdk::Vector2D offset = (VIDEO_MANAGER()->video_size() - image->size())* 0.5f;
        scene_layers_[IMG]->modifier()->set_offset(offset);

        interface_node_->AddChild(scene_layers_[IMG]);
    }
    else scene_layers_[IMG] = NULL;

    ugdk::Engine::reference()->PushInterface(interface_node_);
}

ImageScene::~ImageScene() {
    ugdk::Engine::reference()->RemoveInterface(interface_node_);
    delete interface_node_;
}

void ImageScene::End() {
    super::End();
    set_visible(false);
}

void ImageScene::Update(float delta_t) {
    super::Update(delta_t);
    ugdk::input::InputManager *input = INPUT_MANAGER();
    if (input->KeyPressed(ugdk::input::K_RETURN) || input->KeyPressed(ugdk::input::K_ESCAPE) ||
        input->KeyPressed(ugdk::input::K_KP_ENTER) || input->MouseUp(ugdk::input::M_BUTTON_LEFT))
        Finish();
}

}
