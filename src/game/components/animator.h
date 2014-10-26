#ifndef HORUSEYE_COMPONENT_ANIMATOR_H_
#define HORUSEYE_COMPONENT_ANIMATOR_H_

#include "game/components/direction.h"
#include "game/utils/isometricanimationset.h"
#include "game/components.h"     

#include <ugdk/graphic.h>
#include <ugdk/action/observer.h>

#include <functional>
#include <memory>

namespace component {

class Animator {
  public:
    Animator(const std::string& animation_set);

    void Configure(Graphic*);

    bool ChangeAnimation(utils::AnimtionType type, const Direction& dir);
    bool ChangeAnimation(const std::string& animation_name);

    void AddObserver(ugdk::action::Observer* observer);
    void AddTickFunction(const std::function<void (void)>& tick);
    
    const std::shared_ptr<ugdk::graphic::SpriteAnimationPlayer>& player() { return player_; }

  private:
    void operator=(const Animator&);

    std::shared_ptr<ugdk::graphic::SpriteAnimationPlayer> player_;
    utils::IsometricAnimationSet& isometric_animation_set_;

};  // class Animator

}  // namespace component

#endif  // HORUSEYE_COMPONENT_ANIMATOR_H_
