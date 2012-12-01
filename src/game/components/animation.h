#ifndef HORUSEYE_COMPONENT_ANIMATION_H_
#define HORUSEYE_COMPONENT_ANIMATION_H_

#include <map>
#include <ugdk/portable/tr1.h>
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

class Animation : public Base, private ugdk::action::Observer {
  public:
    typedef std::tr1::function<void (sprite::WorldObject*)> AnimationCallback;
    static const char* DEFAULT_NAME() { return "animation"; }
    static int DEFAULT_ORDER() { return orders::GRAPHIC + 1; }

    Animation(sprite::WorldObject*);
    virtual ~Animation();

    void Update(double dt);

    /// Changes the direction, no callbacks happens.
    void ChangeDirection(const Direction& dir);

    /** Changes the animation type, callbacks happens if there's an actual change.
        The animation is only changed if the new type can interrupt the current animation. */
    bool ChangeAnimation(utils::AnimtionType type);

    /// First changes the direction, then changes the animation type. A wrapper.
    bool ChangeAnimation(utils::AnimtionType type, const Direction& dir);

    /// Stops the current animation, calls callbacks and goes back to the default animation.
    void FinishAnimation();

    bool CanInterrupt(utils::AnimtionType type) const;
    bool IsAnimation(utils::AnimtionType type) const;

    void AddCallback(utils::AnimtionType type, const AnimationCallback& callback) {
        animation_callbacks_[type] = callback;
    }

  private:
    void Tick();
    void executeCallback();
    bool updateGraphic();
    static utils::AnimtionType default_animation_;

    sprite::WorldObject* owner_;

    Direction current_direction_;

    utils::AnimtionType current_animation_;

    std::map<utils::AnimtionType, AnimationCallback> animation_callbacks_;


};  // class Graphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_GRAPHIC_H_
