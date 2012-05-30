#ifndef HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_

#include <ugdk/ui.h>
#include <ugdk/action.h>

namespace builder {

class MenuBuilder {
  public:
    MenuBuilder() {}
    ~MenuBuilder() {}

    ugdk::action::Scene* PauseMenu() const;
    ugdk::action::Scene* MainMenu() const;
};

}

#endif /* HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_ */
