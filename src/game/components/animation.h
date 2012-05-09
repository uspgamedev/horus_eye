#ifndef HORUSEYE_COMPONENT_ANIMATION_H_
#define HORUSEYE_COMPONENT_ANIMATION_H_

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/action/observer.h>
#include <ugdk/math/vector2D.h>

#include "game/components/direction.h"
#include "game/components/graphic.h"

namespace component {

class Animation : public ugdk::action::Observer {
  public:
    enum AnimtionTypes {
        STANDING  = 0,
        WALKING   = 1,
        ATTACKING = 2
    };

    Animation(Graphic* graphic, ugdk::graphic::Spritesheet *spritesheet, ugdk::action::AnimationSet* animation_set);
    virtual ~Animation();

    void Update(double dt);
    void Tick();

    void set_direction(const Direction& dir);
    void select_animation(AnimtionTypes types);

  private:
    static int direction_mapping_[8];

    Graphic* graphic_;

    ugdk::graphic::Sprite* sprite_;

    ugdk::action::AnimationSet* animation_set_;

    ugdk::uint32 animation_index_[3][16];

    Direction current_direction_;
    AnimtionTypes current_animation_;

};  // class Graphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_GRAPHIC_H_
