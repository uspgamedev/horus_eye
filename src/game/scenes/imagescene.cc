#include "imagescene.h"

#include <functional>

#include <ugdk/system/engine.h>
#include <ugdk/action.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/input/events.h>
#include "game/utils/levelmanager.h"

namespace scene {

using ugdk::system::Task;
using ugdk::input::KeyPressedEvent;
using ugdk::input::MouseButtonReleasedEvent;
using ugdk::input::Scancode;
using std::bind;
using namespace std::placeholders;

#define BG  0
#define IMG 1

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

    event_handler().AddListener<KeyPressedEvent>([this](const KeyPressedEvent& ev) {
        if(ev.scancode == Scancode::RETURN || ev.scancode == Scancode::ESCAPE
            || ev.scancode == Scancode::NUMPAD_ENTER)
            this->Finish();
    });
    event_handler().AddListener<MouseButtonReleasedEvent>([this](const MouseButtonReleasedEvent&) {
        this->Finish();
    });
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
