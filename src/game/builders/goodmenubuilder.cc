#include <functional>

#include <ugdk/action/generictask.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/text.h>
#include <ugdk/graphic/drawable/solidrectangle.h>
#include <ugdk/ui/menu.h>
#include <ugdk/ui/uielement.h>

#include "goodmenubuilder.h"

#include "game/scenes/world.h"
#include "game/utils/levelmanager.h"

using std::tr1::bind;
using namespace std::tr1::placeholders;
using ugdk::base::ResourceManager;
using ugdk::graphic::Text;
using ugdk::ui::Menu;
using ugdk::ui::UIElement;

namespace builder {

bool FinishOnEscape(Menu* menu, double dt) {
    ugdk::input::InputManager *input = ugdk::Engine::reference()->input_manager();
    if(input->KeyPressed(ugdk::input::K_ESCAPE)) {
        menu->Finish();
        return false;
    }
    return true;
}

void PauseContinueCallback(Menu* menu, const UIElement * source) {
    menu->Finish();
}

void PauseExitCallback(Menu* menu, const UIElement * source) {
    WORLD()->FinishLevel(utils::LevelManager::FINISH_QUIT);
    menu->Finish();
}

Menu* MenuBuilder::PauseMenu() const {
    ugdk::Vector2D origin(0.0, 0.0), target = VIDEO_MANAGER()->video_size();
    ugdk::ikdtree::Box<2> box(origin.val, target.val);
    Menu* menu = new Menu(box);

    Text* cont_text = ResourceManager::CreateTextFromLanguageTag("Continue");
    Text* exit_text = ResourceManager::CreateTextFromLanguageTag("Return to Menu");

    ugdk::Vector2D cont_position = target * 0.5;
    cont_position.y -= cont_text->size().y;

    ugdk::Vector2D exit_position = target * 0.5;
    exit_position.y += exit_text->size().y;

    UIElement* cont_element = new UIElement(cont_position - cont_text->size() * 0.5, cont_position + cont_text->size() * 0.5, bind(PauseContinueCallback, menu, _1));
    UIElement* exit_element = new UIElement(exit_position - exit_text->size() * 0.5, exit_position + exit_text->size() * 0.5, bind(PauseExitCallback, menu, _1));

    cont_element->node()->set_drawable(cont_text);
    exit_element->node()->set_drawable(exit_text);
    
    menu->AddObject(cont_element);
    menu->AddObject(exit_element);

    menu->StopsPreviousMusic(false);
    menu->AddTask(new ugdk::action::GenericTask(bind(FinishOnEscape, menu, _1)));
    ugdk::graphic::SolidRectangle* bg = new ugdk::graphic::SolidRectangle(target);
    bg->set_color(ugdk::Color(0.5, 0.5, 0.5, 0.5));
    menu->interface_node()->set_drawable(bg);


    return menu;
}

}
