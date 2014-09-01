#include "game/components/animator.h"

#include "game/components/graphic.h"

#include <ugdk/graphic/sprite.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/graphic/primitivesetup.h>
#include "game/utils/isometricanimationset.h"

namespace component {

namespace {
    const ugdk::action::SpriteAnimationTable* IsometricToSet(utils::IsometricAnimationSet* iso_animation_set) {
        return iso_animation_set ? (iso_animation_set->animation_set()) : nullptr;
    }
}
    
Animator::Animator(const std::string& animation_set) 
    :   isometric_animation_set_(utils::IsometricAnimationSet::LoadFromResourceManager(animation_set))
{
}
    
void Animator::Configure(Graphic* graphic) {
    player_ = ugdk::graphic::PrimitiveSetup::Sprite::CreateSpriteAnimationPlayer(graphic->primitive(), isometric_animation_set_.animation_set());
}

bool Animator::ChangeAnimation(utils::AnimtionType type, const Direction& dir) {
    int index = isometric_animation_set_.Get(type, dir);
    if (index > -1) player_->Select(index);
    return true;
}
    
bool Animator::ChangeAnimation(const std::string& animation_name) {
    player_->Select(animation_name);
    return true;
}
    
void Animator::AddObserver(ugdk::action::Observer* observer) {
    player_->AddObserver(observer);
}
    
void Animator::AddTickFunction(const std::function<void (void)>& tick) {
    player_->AddTickFunction(tick);
}
    
}  // namespace component
