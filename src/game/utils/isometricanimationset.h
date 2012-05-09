#ifndef HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_
#define HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_

#include <string>
#include <ugdk/action.h>
#include "game/components/direction.h"

namespace utils {

class IsometricAnimationSet {
  public:
    enum AnimtionType {
        STANDING  = 0,
        WALKING   = 1,
        ATTACKING = 2
    };

    IsometricAnimationSet(ugdk::action::AnimationSet* animation_set);
    virtual ~IsometricAnimationSet();

    ugdk::action::Animation* Get(AnimtionType type, const component::Direction dir);
    ugdk::action::Animation* Search(const std::string& name);

  private:
    ugdk::action::AnimationSet* animation_set_;
    ugdk::uint32 animation_index_[3][16];

};  // class IsometricAnimationSet

}  // namespace utils

#endif  // HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_
