#include "imagescene.h"

#include <functional>

#include <ugdk/system/engine.h>
#include <ugdk/action.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/module.h>
#include <ugdk/ui/node.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/input/events.h>

namespace frontend {
namespace scenes {

using ugdk::system::Task;
using ugdk::input::KeyPressedEvent;
using ugdk::input::MouseButtonReleasedEvent;
using ugdk::input::Scancode;
using std::bind;
using namespace std::placeholders;

#define BG  0
#define IMG 1

ImageScene::ImageScene(std::unique_ptr<ugdk::ui::Drawable>&& background, std::unique_ptr<ugdk::ui::Drawable>&& image) {
    // Node [0], background image
    if (background) {
        scene_layers_[BG].reset(new ugdk::ui::Node(std::move(background)));
        scene_layers_[BG]->set_zindex(-1.0);
    }

    // Node [1], main image
    if (image) {
        ugdk::math::Vector2D offset = (ugdk::graphic::manager()->screen()->size() - image->size())* 0.5;
        scene_layers_[IMG].reset(new ugdk::ui::Node(std::move(image)));
        scene_layers_[IMG]->geometry().set_offset(offset);
    }

    event_handler().AddListener<KeyPressedEvent>([this](const KeyPressedEvent& ev) {
        if(ev.scancode == Scancode::RETURN || ev.scancode == Scancode::ESCAPE
            || ev.scancode == Scancode::NUMPAD_ENTER)
            this->Finish();
    });
    event_handler().AddListener<MouseButtonReleasedEvent>([this](const MouseButtonReleasedEvent&) {
        this->Finish();
    });
    set_render_function([this](ugdk::graphic::Canvas& canvas) {
        if(scene_layers_[BG])
            scene_layers_[BG]->Render(canvas);
        if(scene_layers_[IMG])
            scene_layers_[IMG]->Render(canvas);
    });
}

ImageScene::~ImageScene() {}

void ImageScene::End() {
    super::End();
    this->set_visible(false);
}

} // namespace scenes
} // namespace frontend