#include "imagescene.h"
#include <ugdk/base/engine.h>
#include <ugdk/action/layer.h>
#include <ugdk/action/sprite.h>
#include <ugdk/input/inputmanager.h>
#include "game/utils/levelmanager.h"
#include <ugdk/input/keys.h>

namespace scene {

#define BG  0
#define IMG 1

using namespace ugdk;

ImageScene::ImageScene(ugdk::Drawable *background, ugdk::Drawable *image) 
    :   interface_node_(new Node) {


    // Node [0], background image
    if (background) {
        scene_layers_[BG] = new Node(background);
        scene_layers_[BG]->set_zindex(-1.0f);
        interface_node_->AddChild(scene_layers_[BG]);
    }
    else scene_layers_[BG] = NULL;

    // Node [1], main image
    if (image) {
        scene_layers_[IMG] = new Node(image);

        Vector2D offset = (VIDEO_MANAGER()->video_size() - image->render_size())* 0.5f;
        scene_layers_[IMG]->modifier()->set_offset(offset);

        interface_node_->AddChild(scene_layers_[IMG]);
    }
    else scene_layers_[IMG] = NULL;

    Engine::reference()->PushInterface(interface_node_);
}

ImageScene::~ImageScene() {
    Engine::reference()->RemoveInterface(interface_node_);
    delete interface_node_;
}

void ImageScene::End() {
    super::End();
    set_visible(false);
}

void ImageScene::Update(float delta_t) {
    super::Update(delta_t);
    InputManager *input = Engine::reference()->input_manager();
    if (input->KeyPressed(K_RETURN) || input->KeyPressed(K_ESCAPE) ||
        input->KeyPressed(K_KP_ENTER) || input->MouseUp(M_BUTTON_LEFT))
        Finish();
}

}
