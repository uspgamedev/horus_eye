#ifndef HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONMASK_H_
#define HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONMASK_H_

// Otherwise NULL is undefined...
#include <cstdlib>

namespace sprite {

#define DEFINE_COLLIDABLE public: \
    static const CollisionMask Collision() { return collision_; } \
    virtual const CollisionMask collision() const { return Collision(); } \
  private: \
    static const CollisionMask collision_;

class CollisionMask {
  public:
    // Generates a new CollisionMask with an unique mask.
    static const CollisionMask generate();

    bool operator<(const CollisionMask& other)  const { return identifier_ <  other.identifier_; }
    bool operator<=(const CollisionMask& other) const { return identifier_ <= other.identifier_; }
    bool operator>(const CollisionMask& other)  const { return identifier_ >  other.identifier_; }
    bool operator>=(const CollisionMask& other) const { return identifier_ >= other.identifier_; }
    bool operator==(const CollisionMask& other) const { return identifier_ == other.identifier_; }
    bool operator!=(const CollisionMask& other) const { return identifier_ != other.identifier_; }

    ~CollisionMask() {}
    const CollisionMask *parent() const { return parent_; }
    void set_parent(CollisionMask *parent) { parent_ = parent; }

  private:
    static int identifier_counter_;
    
    CollisionMask(int identifier) : identifier_(identifier), parent_(NULL) {}

    int identifier_;
    CollisionMask *parent_;
};

}

#endif /* HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONMASK_H_ */
