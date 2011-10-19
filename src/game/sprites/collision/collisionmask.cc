#include "collisionmask.h"

namespace sprite {

const CollisionMask CollisionMask::generate() {
    return CollisionMask(identifier_counter_++);
}

int CollisionMask::identifier_counter_ = 0;

}