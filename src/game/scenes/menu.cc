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
#define MENU_TOP            VIDEO_MANAGER()->video_size().y/3.0f
#define MENU_LEFT           VIDEO_MANAGER()->video_size().x/2.0f - RECT_WIDTH/2.0f
#define MENU_BOTTOM         MENU_TOP + Menu::MAIN_SELECT_NUM*RECT_HEIGHT
#define MENU_RIGHT          VIDEO_MANAGER()->video_size().x/2.0f + RECT_WIDTH/2.0f
#define SELECTION_SPRITES   2

const float Menu::OPTION_ZINDEX = 10.0f;

Menu::Menu (int selection_num)
    : selection_(0) {

    selection_num_ = selection_num;
    handler_ = NULL;
    content_box_defined_ = false;

    selection_pos_ = new Vector2D[selection_num_];
    interface_node_ = new Node;
    Engine *engine = Engine::reference();
    engine->PushInterface(interface_node_);

    for (int i = 0; i < SELECTION_SPRITES ; i++)
        interface_node_->AddChild(selection_sprite_[i] = new Node);

    options_sprite_ = new Node*[selection_num_];
    for(int i = 0; i < selection_num_; ++i) {
        interface_node_->AddChild(options_sprite_[i] = new Node);
        options_sprite_[i]->set_zindex(OPTION_ZINDEX);
    }
}

Menu::~Menu () {
    if(handler_ != NULL) {
        handler_->CleanUp();
        delete handler_;
    }
    delete[] selection_pos_;
    delete[] options_sprite_;
    Engine::reference()->RemoveInterface(interface_node_);
    delete interface_node_;
}

void Menu::Update(float delta_t) {
    Scene::Update(delta_t);
    InputManager *input = Engine::reference()->input_manager();
    Vector2D mouse_pos = input->GetMousePosition();

    if (input->KeyPressed(K_ESCAPE)) {
        Finish();
        return;
    }

    if (input->KeyPressed(K_UP))
        do {
            selection_ = selection_ - 1 < 0 ? selection_num_ - 1 : selection_ - 1;
        } while(options_sprite_[selection_] == NULL);
    if (input->KeyPressed(K_DOWN)){
        do {
            selection_ = (selection_ + 1) % selection_num_;
        } while(options_sprite_[selection_] == NULL);
    }

    int modifier = 0;

    if (input->KeyPressed(K_RIGHT)) modifier++;
    if (input->KeyPressed(K_LEFT)) modifier--;

    bool on_selection = CheckMouse(mouse_pos);

    Show();
    Select();

    if (input->KeyPressed(K_RETURN) || modifier ||
        (on_selection && input->MouseUp(M_BUTTON_LEFT)))
        handler_->Handle(selection_, modifier);
}

// Sets the handler the Menu will call on input.
// Calls the handler CleanUp when the menu is deleted.
void Menu::set_handler(MenuHandler* handler) {
    handler_ = handler;
}

void Menu::set_content_box(ugdk::Frame content_box) {
    set_content_box(content_box, ALIGNMENT_CENTER);
}

void Menu::set_content_box(ugdk::Frame content_box, int alignment) {
    content_box_ = content_box;
    content_box_defined_ = true;
    DecideWhereOptionsGo(alignment);
}

void Menu::set_selection_sprite(ugdk::Drawable *drawable) {
    selection_sprite_[0]->set_drawable(drawable);
    selection_sprite_[1]->set_drawable(NULL);
    InitialSelection();
}

void Menu::set_selection_sprite(ugdk::Drawable *drawable[]) {
    selection_sprite_[0]->set_drawable(drawable[0]);
    selection_sprite_[1]->set_drawable(drawable[1]);
    InitialSelection();
}

void Menu::set_option_sprite(int index, ugdk::Drawable *drawable) {
    if (index >= 0 && index < selection_num_ && content_box_defined_) {
        options_sprite_[index]->set_drawable(drawable);
    }
}

void Menu::AddDrawable(ugdk::Drawable *drawable, ugdk::Vector2D pos) {
    ugdk::Node* node = new ugdk::Node(drawable);
    interface_node_->AddChild(node);
    node->modifier()->set_offset(pos);
    node->set_zindex(-OPTION_ZINDEX);
}

void Menu::DecideWhereOptionsGo(int alignment) {
    float selection_height = content_box_.height()/selection_num_;
    float y;
    for (int i = 0; i < selection_num_; ++i) {
        y = content_box_.top() + static_cast<float>(i)*selection_height;
        switch (alignment) {
            case ALIGNMENT_LEFT:
                selection_pos_[i] = Vector2D(content_box_.left(), y);
                break;
            case ALIGNMENT_CENTER:
                selection_pos_[i] = Vector2D((content_box_.right()+content_box_.left()) * 0.5 , y);
                break;
            case ALIGNMENT_RIGHT:
                selection_pos_[i] = Vector2D(content_box_.right(), y);
                break;
        }
        options_sprite_[i]->modifier()->set_offset(selection_pos_[i]);
    }
    InitialSelection();
}

void Menu::InitialSelection() {
    for (int i = 0; i < SELECTION_SPRITES; i++) {
        if (selection_sprite_[i] != NULL && content_box_defined_) {
            selection_sprite_[i]->modifier()->set_offset(selection_pos_[0]);
        }
    }
}

bool Menu::CheckMouse (ugdk::Vector2D &mouse_pos) {

    static float    old_x = 0, old_y = 0;
    float           x = mouse_pos.x,
                    y = mouse_pos.y,
                    dx = x - old_x,
                    dy = y - old_y;
    static bool     on_selection = false;
    float selection_height = content_box_.height()/selection_num_;

    if (dx*dx > 0 || dy*dy > 0 || !visible()) {
        old_x = x;
        old_y = y;
        if ((y >= content_box_.top() && y < content_box_.bottom()) &&
            (x >= content_box_.left() && x < content_box_.right())) {
            int selection = static_cast<int>((y - content_box_.top())/selection_height);
            if((on_selection = (options_sprite_[selection] != NULL)))
                selection_ = selection;
        }
        else on_selection = false;
    }
    return on_selection;
}

void Menu::Select () {
    for (int i = 0; i < SELECTION_SPRITES; i++) {
        if (selection_sprite_[i] != NULL) {
            Vector2D pos = selection_pos_[selection_];
            /*float offset = options_sprite_[selection_]->size().x / 2 ;
            pos.x = pos.x + i * -offset + (1 - i) * offset;*/
            selection_sprite_[i]->modifier()->set_offset(pos);
        }
    }
}

void Menu::Hide() {
    this->set_visible(false);
    interface_node_->set_visible(false);
}

void Menu::Show() {
    this->set_visible(true);
    interface_node_->set_visible(true);
}

void Menu::Toggle() {
    if(this->visible())
        Hide();
    else
        Show();
}

}
