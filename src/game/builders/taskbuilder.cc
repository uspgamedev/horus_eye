#include "taskbuilder.h"

#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include <ugdk/action/generictask.h>
#include <ugdk/base/engine.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/ui/menu.h>
#include <ugdk/math/vector2D.h>

#include "game/scenes/menu.h"
#include "game/builders/goodmenubuilder.h"

static bool VerifyPause(double dt) {
    ugdk::input::InputManager *input = ugdk::Engine::reference()->input_manager();
    if(input->KeyPressed(ugdk::input::K_ESCAPE)) {
        builder::MenuBuilder builder;
        ugdk::Engine::reference()->PushScene(builder.PauseMenu());
    }
    return true;
}

namespace builder {

ugdk::action::Task* TaskBuilder::PauseMenuTask() const {
    return new ugdk::action::GenericTask(VerifyPause);
}

}
