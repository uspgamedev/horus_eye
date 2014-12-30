#ifndef HORUSEYE_COMPONENT_ANIMATION_H_
#define HORUSEYE_COMPONENT_ANIMATION_H_

#include "game/components/base.h"
#include "game/components/direction.h"
#include "game/components/orders.h"

// For utils::AnimtionType
#include "game/utils/isometricanimationset.h"

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/action/observer.h>
#include <ugdk/math/vector2D.h>

#include <map>
#include <functional>

namespace component {

class Animation : public Base, private ugdk::action::Observer {
  public:
    typedef std::function<void (sprite::WObjRawPtr)> AnimationCallback;
    static const char* DEFAULT_NAME() { return "animation"; }
    static int DEFAULT_ORDER() { return orders::GRAPHIC + 1; }

    Animation();
    Animation(utils::AnimtionType type, const Direction& dir);
    virtual ~Animation();

    void Update(double dt) override;
    void OnAdd(sprite::WObjRawPtr) override;

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

    sprite::WObjRawPtr owner_;

    Direction current_direction_;

    utils::AnimtionType current_animation_;

    std::map<utils::AnimtionType, AnimationCallback> animation_callbacks_;


};  // class Graphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_GRAPHIC_H_
