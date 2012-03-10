#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>

#include "follower.h"

#include "game/scenes/world.h"
#include "game/utils/tile.h"
#include "game/utils/constants.h"

#define LIGHT_COEFFICIENT 0.75

namespace sprite {

using namespace ugdk;
using namespace scene;
using namespace utils;

Follower::Follower(WorldObject* target, double duration)
  : WorldObject(duration), target_(target) {}

void Follower::Update(double dt) {
    super::Update(dt);
    if((target_->world_position() - world_position()).LengthSquared() > 1.0e-6)
        this->set_world_position(target_->world_position());
}

}  // namespace sprite
