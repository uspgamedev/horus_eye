#ifndef HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONMASK_H_
#define HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONMASK_H_

namespace sprite {

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

  private:
    static int identifier_counter_;
    
    CollisionMask(int identifier) : identifier_(identifier) {}
    int identifier_;
};

}

#endif /* HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONMASK_H_ */
