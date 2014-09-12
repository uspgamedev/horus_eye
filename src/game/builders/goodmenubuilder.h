#ifndef HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_

#include <ugdk/action.h>
#include <memory>

namespace builder {

std::unique_ptr<ugdk::action::Scene> PauseMenu();
std::unique_ptr<ugdk::action::Scene> MainMenu();
std::unique_ptr<ugdk::action::Scene> SettingsMenu();

}

#endif /* HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_ */
