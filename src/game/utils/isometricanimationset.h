#ifndef HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_
#define HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_

#include <string>
#include <ugdk/base/types.h>
#include <ugdk/action.h>
#include "game/components/direction.h"

namespace utils {

enum AnimtionType {
    STANDING  = 0,
    WALKING   = 1,
    ATTACKING = 2,
    DYING     = 3,
    TAKING_HIT = 4
};

class IsometricAnimationSet {
  public:
    IsometricAnimationSet(ugdk::action::AnimationSet* animation_set);
    virtual ~IsometricAnimationSet();

    int Get(AnimtionType type, const component::Direction dir);
    
    ugdk::action::AnimationSet* animation_set() { return animation_set_; }

  private:
    ugdk::action::AnimationSet* animation_set_;
    ugdk::uint32 animation_index_[5][16];

};  // class IsometricAnimationSet

}  // namespace utils

#endif  // HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_
