#include "game/components/animator.h"

#include "game/components/graphic.h"
#include "game/sprites/worldobject.h"

#include <ugdk/graphic/sprite.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/graphic/spritetypes.h>
#include <ugdk/graphic/primitivesetup.h>
#include "game/utils/isometricanimationset.h"

namespace component {

Animator::Animator(const std::string& animation_set) 
    :   isometric_animation_set_(utils::IsometricAnimationSet::LoadFromResourceManager(animation_set))
{
    player_.reset(new ugdk::graphic::SpriteAnimationPlayer(isometric_animation_set_.animation_set()));
}
    
void Animator::Configure(Graphic* graphic) {
    auto p = &graphic->primitive();
    player_->set_frame_change_callback([p](const ugdk::graphic::SpriteAnimationFrame& frame) {
        if (auto sprite = dynamic_cast<ugdk::graphic::PrimitiveControllerSprite*>(p->controller().get())) {
            sprite->ChangeToAnimationFrame(frame);
        }
    });
    player_->Refresh();
}

void Animator::Update(double dt) {
    player_->Update(dt);
}

void Animator::OnAdd(sprite::WorldObject* wobj) {
    if (auto graphic = wobj->graphic())
        Configure(graphic);
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
