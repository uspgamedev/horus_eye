#include "follower.h"
#include "game/sprites/worldobject.h"

namespace component {

void Follower::Update(double dt) {
    if((target_->world_position() - owner_->world_position()).LengthSquared() > 1.0e-6)
        owner_->set_world_position(target_->world_position());
}

}  // namespace component
