#include <cstdlib>
#include <string>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/input/inputmanager.h>

#include "menu.h"

#include "game/utils/levelloader.h"
#include "game/utils/levelmanager.h"
#include "game/scenes/imagescene.h"

namespace scene {

using namespace ugdk;
using namespace utils;

#define RECT_WIDTH          266
#define RECT_HEIGHT         90
#define SELECTION_WIDTH     864
#define SELECTION_HEIGHT    155
#define MENU_TOP            VIDEO_MANAGER()->video_size().y/3.0
#define MENU_LEFT           VIDEO_MANAGER()->video_size().x/2.0 - RECT_WIDTH/2.0
#define MENU_BOTTOM         MENU_TOP + Menu::MAIN_SELECT_NUM*RECT_HEIGHT
#define MENU_RIGHT          VIDEO_MANAGER()->video_size().x/2.0 + RECT_WIDTH/2.0
#define SELECTION_SPRITES   2

const double Menu::OPTION_ZINDEX = 10.0;

Menu::Menu (int selection_num)
    : selection_(0) {

    selection_num_ = selection_num;
    handler_ = NULL;
    content_box_defined_ = false;

    selection_pos_ = new Vector2D[selection_num_];
    interface_node_ = new ugdk::graphic::Node;
    Engine *engine = Engine::reference();
    engine->PushInterface(interface_node_);

    for (int i = 0; i < SELECTION_SPRITES ; i++)
        interface_node_->AddChild(selection_node_[i] = new ugdk::graphic::Node);

    options_node_ = new ugdk::graphic::Node*[selection_num_];
    for(int i = 0; i < selection_num_; ++i) {
        interface_node_->AddChild(options_node_[i] = new ugdk::graphic::Node);
        options_node_[i]->set_zindex(OPTION_ZINDEX);
    }
}

Menu::~Menu () {
    if(handler_ != NULL) {
        handler_->CleanUp();
        delete handler_;
    }
    delete[] selection_pos_;
    delete[] options_node_;
    Engine::reference()->RemoveInterface(interface_node_);
    delete interface_node_;
}

void Menu::Update(double delta_t) {
    Scene::Update(delta_t);
    ugdk::input::InputManager *input = Engine::reference()->input_manager();
    Vector2D mouse_pos = input->GetMousePosition();

    if (input->KeyPressed(ugdk::input::K_ESCAPE)) {
        Finish();
        return;
    }

    if (input->KeyPressed(ugdk::input::K_UP))
        do {
            selection_ = selection_ - 1 < 0 ? selection_num_ - 1 : selection_ - 1;
        } while(options_node_[selection_]->drawable() == NULL);
    if (input->KeyPressed(ugdk::input::K_DOWN)){
        do {
            selection_ = (selection_ + 1) % selection_num_;
        } while(options_node_[selection_]->drawable() == NULL);
    }

    int modifier = 0;

    if (input->KeyPressed(ugdk::input::K_RIGHT)) modifier++;
    if (input->KeyPressed(ugdk::input::K_LEFT)) modifier--;

    bool on_selection = CheckMouse(mouse_pos);

    Show();
    Select();

    if (input->KeyPressed(ugdk::input::K_RETURN) || modifier ||
        (on_selection && input->MouseUp(ugdk::input::M_BUTTON_LEFT)))
        handler_->Handle(selection_, modifier);
}

// Sets the handler the Menu will call on input.
// Calls the handler CleanUp when the menu is deleted.
void Menu::set_handler(MenuHandler* handler) {
    handler_ = handler;
}

void Menu::set_content_box(ugdk::Frame content_box) {
	set_content_box(content_box, ugdk::graphic::Drawable::CENTER);
}

void Menu::set_content_box(ugdk::Frame content_box, ugdk::graphic::Drawable::HookPoint alignment) {
    content_box_ = content_box;
    content_box_defined_ = true;
    DecideWhereOptionsGo(alignment);
}

void Menu::set_selection_sprite(ugdk::graphic::Drawable *drawable) {
    selection_node_[0]->set_drawable(drawable);
    selection_node_[1]->set_drawable(NULL);

	drawable->set_hotspot(option_alignment_);
}

void Menu::set_selection_sprite(ugdk::graphic::Drawable **drawable) {
    selection_node_[0]->set_drawable(drawable[0]);
    selection_node_[1]->set_drawable(drawable[1]);

	drawable[0]->set_hotspot(option_alignment_);
	drawable[1]->set_hotspot(option_alignment_);
}

void Menu::set_option_sprite(int index, ugdk::graphic::Drawable *drawable) {
    if (index >= 0 && index < selection_num_ && content_box_defined_) {
        options_node_[index]->set_drawable(drawable);
        options_node_[index]->modifier()->set_offset(selection_pos_[index]);
        drawable->set_hotspot(option_alignment_);
    }
}

void Menu::AddDrawable(ugdk::graphic::Drawable *drawable, ugdk::Vector2D pos) {
    ugdk::graphic::Node* node = new ugdk::graphic::Node(drawable);
    interface_node_->AddChild(node);
    node->modifier()->set_offset(pos);
    node->set_zindex(-OPTION_ZINDEX);
}

void Menu::AddNode(ugdk::graphic::Node *node) {
    interface_node_->AddChild(node);
    node->set_zindex(-OPTION_ZINDEX);
}

void Menu::DecideWhereOptionsGo(ugdk::graphic::Drawable::HookPoint alignment) {
	option_alignment_ = alignment;

    double height = content_box_.height()/selection_num_;
	double width = content_box_.right()-content_box_.left();

	Vector2D offset;
    switch (option_alignment_) {
		case ugdk::graphic::Drawable::TOP_LEFT    : offset = Vector2D(        0.0,          0.0); break;
        case ugdk::graphic::Drawable::TOP         : offset = Vector2D(width * 0.5,          0.0); break;
        case ugdk::graphic::Drawable::TOP_RIGHT   : offset = Vector2D(       width,          0.0); break;
        case ugdk::graphic::Drawable::LEFT        : offset = Vector2D(        0.0, height * 0.5); break;
        case ugdk::graphic::Drawable::CENTER      : offset = Vector2D(width * 0.5, height * 0.5); break;
        case ugdk::graphic::Drawable::RIGHT       : offset = Vector2D(       width, height * 0.5); break;
        case ugdk::graphic::Drawable::BOTTOM_LEFT : offset = Vector2D(        0.0,         height); break;
        case ugdk::graphic::Drawable::BOTTOM      : offset = Vector2D(width * 0.5,         height); break;
        case ugdk::graphic::Drawable::BOTTOM_RIGHT: offset = Vector2D(       width,         height); break;
    }
    for (int i = 0; i < selection_num_; ++i) {
        double y = content_box_.top() + static_cast<double>(i)*height;
		selection_pos_[i] = Vector2D(content_box_.left(), y) + offset;
    }

    for (int i = 0; i < SELECTION_SPRITES; i++)
        selection_node_[i]->modifier()->set_offset(selection_pos_[0]);
}

bool Menu::CheckMouse (ugdk::Vector2D &mouse_pos) {

    static double    old_x = 0, old_y = 0;
    double           x = mouse_pos.x,
                    y = mouse_pos.y,
                    dx = x - old_x,
                    dy = y - old_y;
    static bool     on_selection = false;
    double selection_height = content_box_.height()/selection_num_;

    if (dx*dx > 0 || dy*dy > 0 || !visible()) {
        old_x = x;
        old_y = y;
        if ((y >= content_box_.top() && y < content_box_.bottom()) &&
            (x >= content_box_.left() && x < content_box_.right())) {
            int selection = static_cast<int>((y - content_box_.top())/selection_height);
            if((on_selection = (options_node_[selection]->drawable() != NULL)))
                selection_ = selection;
        }
        else on_selection = false;
    }
    return on_selection;
}

void Menu::Select () {
    for (int i = 0; i < SELECTION_SPRITES; i++) {
        Vector2D pos = selection_pos_[selection_];
        double offset = (options_node_[selection_]->drawable()->size().x + selection_node_[i]->drawable()->size().x) * 0.5;
        pos.x = pos.x + i * -offset + (1 - i) * offset; // awesum dodge of if
        selection_node_[i]->modifier()->set_offset(pos);
    }
}

void Menu::Hide() {
    this->set_visible(false);
    interface_node_->modifier()->set_visible(false);
}

void Menu::Show() {
    this->set_visible(true);
    interface_node_->modifier()->set_visible(true);
}

void Menu::Toggle() {
    if(this->visible())
        Hide();
    else
        Show();
}

}
