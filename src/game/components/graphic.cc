#include "game/components/graphic.h"

#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spritetypes.h>
#include "game/utils/isometricanimationset.h"

namespace component {

using ugdk::graphic::Sprite;

Graphic::Graphic(const std::string& spritesheet_tag, utils::IsometricAnimationSet* iso_animation_set, double light_radius)
    :   BaseGraphic(NULL, light_radius),
        sprite_(new Sprite(spritesheet_tag, iso_animation_set ? (iso_animation_set->animation_set()) : NULL)),
        isometric_animation_set_(iso_animation_set) {

    node_->set_drawable(sprite_);
}

Graphic::~Graphic() {}

bool Graphic::ChangeAnimation(utils::AnimtionType type, const Direction& dir) {
    if(!isometric_animation_set_) return false;
    int index = isometric_animation_set_->Get(type, dir);
    if(index > -1) sprite_->animation_player().Select(index);
    return true;
}

void Graphic::AddObserver(ugdk::action::Observer* observer) {
    sprite_->animation_player().AddObserver(observer);

}
    
}  // namespace component
