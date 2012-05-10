#ifndef HORUSEYE_COMPONENT_ANIMATION_H_
#define HORUSEYE_COMPONENT_ANIMATION_H_

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/action/observer.h>
#include <ugdk/math/vector2D.h>

#include "game/components/direction.h"
#include "game/components/graphic.h"
#include "game/utils/isometricanimationset.h"

namespace component {

class Animation : public ugdk::action::Observer {
  public:
    Animation(Graphic* graphic, ugdk::graphic::Spritesheet *spritesheet, utils::IsometricAnimationSet* animation_set);
    virtual ~Animation();

    void Update(double dt);
    void Tick();

    void set_direction(const Direction& dir);
    void set_animation(utils::AnimtionType type);
    void queue_animation(utils::AnimtionType type);

    bool has_queued_animation() const { return has_queued_animation_; }
    void flag_uninterrutible() { uninterrutible_ = true; }
    bool is_uninterrutible() const { return uninterrutible_; }

  private:
    static int direction_mapping_[8];

    Graphic* graphic_;

    ugdk::graphic::Sprite* sprite_;

    utils::IsometricAnimationSet* isometric_animation_set_;

    Direction current_direction_;
    utils::AnimtionType current_animation_, queued_animation_;
    bool has_queued_animation_;
    bool uninterrutible_;


};  // class Graphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_GRAPHIC_H_
