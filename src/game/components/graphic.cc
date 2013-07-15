#include "game/components/graphic.h"

#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spritetypes.h>
#include "game/utils/isometricanimationset.h"

namespace component {

using ugdk::graphic::Sprite;
    
Graphic::Graphic(const std::string& spritesheet_tag, const std::string& animation_set, double light_radius)
    :   BaseGraphic(NULL, light_radius) {

    isometric_animation_set_ = utils::IsometricAnimationSet::LoadFromResourceManager(animation_set);
    sprite_ = new Sprite(spritesheet_tag, isometric_animation_set_ ? (isometric_animation_set_->animation_set()) : NULL);
    ChangeDrawable(sprite_);
}

Graphic::Graphic(const std::string& spritesheet_tag, utils::IsometricAnimationSet* iso_animation_set, double light_radius)
    :   BaseGraphic(NULL, light_radius),
        sprite_(new Sprite(spritesheet_tag, iso_animation_set ? (iso_animation_set->animation_set()) : NULL)),
        isometric_animation_set_(iso_animation_set) {

    ChangeDrawable(sprite_);
}
    
Graphic::Graphic(const ugdk::graphic::Spritesheet* spritesheet, utils::IsometricAnimationSet* iso_animation_set, double light_radius)
    :   BaseGraphic(NULL, light_radius),
        sprite_(new Sprite(spritesheet, iso_animation_set ? (iso_animation_set->animation_set()) : NULL)),
        isometric_animation_set_(iso_animation_set) {

    ChangeDrawable(sprite_);
}

Graphic::~Graphic() {}

bool Graphic::ChangeAnimation(utils::AnimtionType type, const Direction& dir) {
    if(!isometric_animation_set_) return false;
    int index = isometric_animation_set_->Get(type, dir);
    if(index > -1) sprite_->animation_player().Select(index);
    return true;
}
    
bool Graphic::ChangeAnimation(const std::string& animation_name) {
    if(!isometric_animation_set_) return false;
    sprite_->animation_player().Select(animation_name);
    return true;
}

void Graphic::Update(double dt) {
    BaseGraphic::Update(dt);
    sprite_->animation_player().Update(dt);
}
    
ugdk::action::SpriteAnimationPlayer& Graphic::animation_player() {
    return sprite_->animation_player();
}

void Graphic::AddObserver(ugdk::action::Observer* observer) {
    sprite_->animation_player().AddObserver(observer);
}
    
void Graphic::AddTickFunction(const std::function<void (void)>& tick) {
    sprite_->animation_player().AddTickFunction(tick);
}
    
}  // namespace component
