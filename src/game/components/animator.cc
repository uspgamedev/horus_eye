#include "game/components/animator.h"

#include <ugdk/graphic/sprite.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spritetypes.h>
#include "game/utils/isometricanimationset.h"

namespace component {

using ugdk::graphic::Sprite;

namespace {
    const ugdk::action::SpriteAnimationTable* IsometricToSet(utils::IsometricAnimationSet* iso_animation_set) {
        return iso_animation_set ? (iso_animation_set->animation_set()) : nullptr;
    }
}
    
Animator::Animator(const std::string& spritesheet_tag, const std::string& animation_set) 
    :   isometric_animation_set_(utils::IsometricAnimationSet::LoadFromResourceManager(animation_set))
{
    sprite_.reset(Sprite::Create(spritesheet_tag, isometric_animation_set_.animation_set()));
}

Animator::Animator(const ugdk::graphic::Spritesheet* spritesheet, const std::string& animation_set)
    :   isometric_animation_set_(utils::IsometricAnimationSet::LoadFromResourceManager(animation_set))
{
    sprite_.reset(new Sprite(spritesheet, isometric_animation_set_.animation_set()));
}

bool Animator::ChangeAnimation(utils::AnimtionType type, const Direction& dir) {
    int index = isometric_animation_set_.Get(type, dir);
    if(index > -1) sprite_->animation_player().Select(index);
    return true;
}
    
bool Animator::ChangeAnimation(const std::string& animation_name) {
    sprite_->animation_player().Select(animation_name);
    return true;
}
    
void Animator::AddObserver(ugdk::action::Observer* observer) {
    sprite_->animation_player().AddObserver(observer);
}
    
void Animator::AddTickFunction(const std::function<void (void)>& tick) {
    sprite_->animation_player().AddTickFunction(tick);
}
    
}  // namespace component
