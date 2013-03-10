#ifndef HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_
#define HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_

#include <string>
#include <ugdk/base/types.h>
#include <ugdk/action.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/base/types.h>
#include "game/components/direction.h"

namespace utils {

enum AnimtionType {
    // Note: order is relevant
    IDLE,
    MOVEMENT,
    ATTACK,
    TAKING_HIT,
    DEATH,

    NUM_ANIMATION_TYPES // Conveniently calculated by the compiler
};

class IsometricAnimationSet {
  public:
    IsometricAnimationSet(ugdk::action::SpriteAnimationTable* animation_set);
    virtual ~IsometricAnimationSet();

    int Get(AnimtionType type, const component::Direction& dir);
    
    ugdk::action::SpriteAnimationTable* animation_set() { return animation_set_; }

    static IsometricAnimationSet* LoadFromFile(const std::string& name);
    static IsometricAnimationSet* LoadFromResourceManager(const std::string& name);

  private:
    ugdk::action::SpriteAnimationTable* animation_set_;
    ugdk::uint32 animation_index_[NUM_ANIMATION_TYPES][16];

};  // class IsometricAnimationSet

}  // namespace utils

#endif  // HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_
