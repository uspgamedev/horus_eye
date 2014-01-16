#ifndef HORUSEYE_COMPONENT_GRAPHIC_H_
#define HORUSEYE_COMPONENT_GRAPHIC_H_

#include "game/components/basegraphic.h"

#include <functional>
#include <ugdk/graphic.h>
#include <ugdk/action/observer.h>

#include "game/components/orders.h"
#include "game/components/direction.h"
#include "game/utils/isometricanimationset.h"

namespace component {

class Graphic : public BaseGraphic {
  public:
    static const char* DEFAULT_NAME() { return "graphic"; }
    static int DEFAULT_ORDER() { return orders::GRAPHIC; }

    Graphic(const std::string& spritesheet_tag, const std::string& animation_set);
    Graphic(const std::string& spritesheet_tag, utils::IsometricAnimationSet* animation_set);
    Graphic(const ugdk::graphic::Spritesheet* spritesheet, utils::IsometricAnimationSet* animation_set);
    virtual ~Graphic();

    void Update(double);

    bool ChangeAnimation(utils::AnimtionType type, const Direction& dir);
    bool ChangeAnimation(const std::string& animation_name);

    ugdk::action::SpriteAnimationPlayer& animation_player();
    void AddObserver(ugdk::action::Observer* observer);
    void AddTickFunction(const std::function<void (void)>& tick);

  private:
    ugdk::graphic::Sprite* sprite_;

    utils::IsometricAnimationSet* isometric_animation_set_;

};  // class Graphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_GRAPHIC_H_
