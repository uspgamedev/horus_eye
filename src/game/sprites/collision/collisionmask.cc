#include "collisionmask.h"

namespace sprite {

const CollisionMask* CollisionMask::generate(CollisionMask *parent) {
    int identifier = identifier_counter_++;
    cache_[identifier] = new CollisionMask(identifier, parent);
    return cache_[identifier];
}

int CollisionMask::identifier_counter_ = 0;

}