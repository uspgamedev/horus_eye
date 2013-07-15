#ifndef HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_
#define HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_

#include <string>
#include <map>
#include <ugdk/action.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/common/types.h>
#include "game/components/direction.h"

namespace utils {

enum AnimtionType {
    // Note: order is relevant
    IDLE,
    MOVEMENT,
    ATTACK,
    TAKING_HIT,
    SPAWNING,
    DEATH,

    NUM_ANIMATION_TYPES // Conveniently calculated by the compiler
};

class IsometricAnimationSet {
  public:
    IsometricAnimationSet(ugdk::action::SpriteAnimationTable* animation_set);
    virtual ~IsometricAnimationSet();

    int Get(AnimtionType type, const component::Direction& dir) const;
    
    ugdk::action::SpriteAnimationTable* animation_set() { return animation_set_; }

    static IsometricAnimationSet* LoadFromFile(const std::string& name);
    static IsometricAnimationSet* LoadFromResourceManager(const std::string& name);

  private:
    typedef std::map<component::Direction, int> AnimationDirectionCache;
    int getAnimationFromCache(const AnimationDirectionCache& cache, const component::Direction& dir) const;

    ugdk::action::SpriteAnimationTable* animation_set_;
    AnimationDirectionCache animation_cache_[NUM_ANIMATION_TYPES];

};  // class IsometricAnimationSet

}  // namespace utils

#endif  // HORUSEYE_UTILS_ISOMETRICANIMATIONSET_H_
