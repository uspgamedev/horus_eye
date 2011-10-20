#include "collisionmask.h"

namespace sprite {

const CollisionMask* CollisionMask::Generate(const CollisionMask *parent) {
    int identifier = identifier_counter_++;
    //cache_[identifier] = 
    return new CollisionMask(identifier, parent);
}

int CollisionMask::identifier_counter_ = 0;

}