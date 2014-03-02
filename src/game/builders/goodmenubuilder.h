#ifndef HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_

#include <ugdk/action.h>

namespace builder {

ugdk::action::Scene* PauseMenu();
ugdk::action::Scene* MainMenu();
ugdk::action::Scene* SettingsMenu();

}

#endif /* HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_ */
