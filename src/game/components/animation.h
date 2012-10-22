#ifndef HORUSEYE_COMPONENT_ANIMATION_H_
#define HORUSEYE_COMPONENT_ANIMATION_H_

#include <map>
#include <ugdk/portable/tr1.h>
#include <map>
#include FROM_TR1(functional)

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/action/observer.h>
#include <ugdk/math/vector2D.h>

#include "game/components/base.h"
#include "game/components/direction.h"
#include "game/sprites/worldobject.h"
#include "game/utils/isometricanimationset.h"
#include "game/components/orders.h"

namespace component {

class Animation : public Base, public ugdk::action::Observer {
  public:
    typedef std::tr1::function<void (sprite::WorldObject*)> AnimationCallback;
    static const char* DEFAULT_NAME() { return "animation"; }
    static int DEFAULT_ORDER() { return orders::GRAPHIC + 1; }

    Animation(sprite::WorldObject*, const std::string& spritesheet_tag,
              utils::IsometricAnimationSet* animation_set);
    virtual ~Animation();

    void Update(double dt);
    void Tick();

    void ChangeDirection(const Direction& dir);
    bool ChangeAnimation(utils::AnimtionType type);
    bool ChangeAnimation(utils::AnimtionType type, const Direction& dir);

    bool CanInterrupt(utils::AnimtionType type) const;
    bool IsAnimation(utils::AnimtionType type) const;

    void AddCallback(utils::AnimtionType type, const AnimationCallback& callback) {
        animation_callbacks_[type] = callback;
    }

  private:
    static int direction_mapping_[8];

    sprite::WorldObject* owner_;

    ugdk::graphic::Sprite* sprite_;

    utils::IsometricAnimationSet* isometric_animation_set_;

    Direction current_direction_;

    utils::AnimtionType current_animation_;

    std::map<utils::AnimtionType, AnimationCallback> animation_callbacks_;


};  // class Graphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_GRAPHIC_H_
