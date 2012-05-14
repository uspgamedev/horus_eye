#include <functional>

#include <ugdk/action/generictask.h>
#include <ugdk/action/scene.h>
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
using ugdk::action::Scene;
using ugdk::ui::UIElement;

namespace builder {

void PauseContinueCallback(Scene* menu, const UIElement * source) {
    menu->Finish();
}

void PauseExitCallback(Scene* menu, const UIElement * source) {
    menu->Finish();
    WORLD()->FinishLevel(utils::LevelManager::FINISH_QUIT);
}

//void FinishMenuCallback(Menu* menu) {
//    menu->FinishScene(menu);
//}

Scene* MenuBuilder::PauseMenu() const {
    ugdk::action::Scene* pause_menu = new Scene();
    ugdk::Vector2D origin(0.0, 0.0), target = VIDEO_MANAGER()->video_size();
    ugdk::ikdtree::Box<2> box(origin.val, target.val);
    Menu* menu = new Menu(box, Vector2D(0.0, 0.0), pause_menu);

    Text* cont_text = ResourceManager::CreateTextFromLanguageTag("Continue");
    Text* exit_text = ResourceManager::CreateTextFromLanguageTag("Return to Menu");
    cont_text->set_hotspot(ugdk::graphic::Drawable::CENTER);

    ugdk::Vector2D cont_position = target * 0.5;
    cont_position.y -= cont_text->size().y;

    ugdk::Vector2D exit_position = target * 0.5;
    exit_position.y += exit_text->size().y;

    UIElement* cont_element = new UIElement(cont_position, menu, bind(PauseContinueCallback, pause_menu, _1));
    UIElement* exit_element = new UIElement(exit_position - exit_text->size() * 0.5, menu, bind(PauseExitCallback, pause_menu, _1));

    cont_element->set_drawable(cont_text);
    exit_element->set_drawable(exit_text);
    
    menu->AddObject(cont_element);
    menu->AddObject(exit_element);

    pause_menu->StopsPreviousMusic(false);
    menu->AddCallback(ugdk::input::K_ESCAPE, ugdk::ui::FINISH_MENU);
    menu->AddCallback(ugdk::input::K_RETURN, &ugdk::ui::Menu::InteractWithFocused);
    ugdk::graphic::SolidRectangle* bg = new ugdk::graphic::SolidRectangle(target);
    bg->set_color(ugdk::Color(0.5, 0.5, 0.5, 0.5));
    pause_menu->interface_node()->set_drawable(bg);
    pause_menu->interface_node()->AddChild(menu->node());

    pause_menu->AddEntity(menu);
    return pause_menu;
}

}
