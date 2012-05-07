#ifndef HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_

#include <ugdk/ui.h>

namespace builder {

class MenuBuilder {
  public:
    MenuBuilder() {}
    ~MenuBuilder() {}

    ugdk::ui::Menu* PauseMenu() const;
};

}

#endif /* HORUSEYE_GAME_BUILDERS_MENUBUILDER_H_ */
