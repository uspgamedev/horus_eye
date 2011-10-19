#ifndef HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONMASK_H_
#define HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONMASK_H_

#include <map>

namespace sprite {

#define DEFINE_COLLIDABLE public: \
    static const CollisionMask* Collision() { return collision_; } \
    virtual const CollisionMask* collision() const { return Collision(); } \
  private: \
    static const CollisionMask* collision_;


#define INITIALIZE_COLLIDABLE(CLASS, parent) const CollisionMask* CLASS::collision_ = CollisionMask::generate(parent);
    
class CollisionMask {
  public:
    // Generates a new CollisionMask with an unique mask.
    static const CollisionMask* generate(CollisionMask *parent = NULL);
    static const CollisionMask* Find(int id) { return cache_[id]; }

    ~CollisionMask() {}
    CollisionMask* parent() const { return parent_; }
    void set_parent(CollisionMask* parent) { parent_ = parent; }

  private:
    static int identifier_counter_;
    CollisionMask(int identifier, CollisionMask* parent = NULL) : identifier_(identifier), parent_(parent) {}

    int identifier_;
    CollisionMask* parent_;
    static std::map<int, CollisionMask*> cache_;
};

}

#endif /* HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONMASK_H_ */
