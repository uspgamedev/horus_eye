#ifndef HORUSEYE_COMPONENT_ANIMATION_H_
#define HORUSEYE_COMPONENT_ANIMATION_H_

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/action/observer.h>
#include <ugdk/math/vector2D.h>

#include "game/components/graphic.h"

namespace component {

class Animation : public ugdk::Observer {
  public:
    Animation(Graphic* graphic, ugdk::graphic::Spritesheet *spritesheet, ugdk::AnimationSet* animation_set);
    virtual ~Animation();

    void Update(double dt);

  private:
    static int direction_mapping_[8];
    struct Direction_ {
        static const int RIGHT = 0;
        static const int LEFT = 1;
        static const int UP = 2;
        static const int DOWN = 3;
    };
    struct Animation_ {
        static const int RIGHT = 1;
        static const int LEFT = 2;
        static const int UP = 4;
        static const int DOWN = 8;
    };
    enum AnimtionTypes_ {
        STANDING  = 0,
        WALKING   = 1,
        ATTACKING = 2
    };

    Graphic* graphic_;

    ugdk::graphic::Sprite* sprite_;

    ugdk::AnimationSet* animation_set_;

    ugdk::uint32 animation_index_[3][16];

};  // class Graphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_GRAPHIC_H_
