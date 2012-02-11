#include <ugdk/base/engine.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable/solidrectangle.h>
#include <ugdk/input/inputmanager.h>

#include "pausescene.h"

namespace scene {

PauseScene::PauseScene () {
    ugdk::graphic::SolidRectangle* rect = new ugdk::graphic::SolidRectangle(VIDEO_MANAGER()->video_size());
    rect->set_color(ugdk::Color(0.5, 0.5, 0.5));

    ugdk::graphic::Node* node = new ugdk::graphic::Node(rect);
    node->modifier()->set_alpha(0.5);

    this->root_node()->AddChild(node);
}

PauseScene::~PauseScene () {}

void PauseScene::Update (double delta_t) {
    if (INPUT_MANAGER()->KeyPressed(ugdk::input::K_RETURN))
        Finish();
}

}
