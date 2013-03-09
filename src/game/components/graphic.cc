#include "game/components/graphic.h"

#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spritetypes.h>
#include "game/utils/isometricanimationset.h"

namespace component {

using ugdk::graphic::Sprite;
using ugdk::action::SpriteTableAnimationPlayer;

Graphic::Graphic(const std::string& spritesheet_tag, utils::IsometricAnimationSet* iso_animation_set, double light_radius)
    :   BaseGraphic(NULL, light_radius),
        isometric_animation_set_(iso_animation_set){

    animation_player_ = iso_animation_set ? (new SpriteTableAnimationPlayer(iso_animation_set->animation_set())) : NULL;
    node_->set_drawable(sprite_);
}

Graphic::~Graphic() {}

bool Graphic::ChangeAnimation(utils::AnimtionType type, const Direction& dir) {
    if(!isometric_animation_set_) return false;
    int index = isometric_animation_set_->Get(type, dir);
    if(index > -1) animation_player_->Select(index);
    return true;
}

void Graphic::AddObserver(ugdk::action::Observer* observer) {
    animation_player_->AddObserver(observer);

}
    
}  // namespace component
