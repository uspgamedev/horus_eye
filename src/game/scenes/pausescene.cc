#include <ugdk/base/engine.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable/solidrectangle.h>
#include <ugdk/input/inputmanager.h>

#include "pausescene.h"

namespace scene {

using namespace ugdk;

PauseScene::PauseScene () {
    SolidRectangle* rect = new SolidRectangle(VIDEO_MANAGER()->video_size());
    rect->set_color(ugdk::Color(0.5f, 0.5f, 0.5f));

    Node* node = new Node(rect);
    node->modifier()->set_alpha(0.5f);

    this->root_node()->AddChild(node);
}

PauseScene::~PauseScene () {}

void PauseScene::Update (float delta_t) {
    InputManager *input = Engine::reference()->input_manager();
    if (input->KeyPressed(K_RETURN)) Finish();
}

}
