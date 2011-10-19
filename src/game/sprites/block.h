#ifndef HORUSEYE_GAME_SPRITE_BLOCK_H_
#define HORUSEYE_GAME_SPRITE_BLOCK_H_

#include <ugdk/math/vector2D.h>
#include "game/sprites/worldobject.h"

namespace ugdk {
class Image;
}

namespace sprite {

class Projectile;
class Wall;
class Block : public WorldObject {
  public:
    Block(ugdk::Image* image);
    ~Block();

    virtual void Update(float dt);
    

    virtual void CollidesWith(Projectile * obj);
    virtual void CollidesWith(Wall * obj) { RevertPosition(); }
    virtual void CollidesWith(Door * obj) { RevertPosition(); }
    virtual void CollidesWith(Block * obj) { RevertPosition(); }
    

    enum Direction { LEFT, RIGHT, UP, DOWN };

    static const CollisionMask Collision() { return collision_; }
    virtual const CollisionMask collision() const { return Block::Collision(); }

  private:
    static const CollisionMask collision_;
#ifdef DEBUG
    void GetKeys();
#endif
    void MoveBlock(float dt);
    void RevertPosition();

    bool moving_;
    float moving_time_left_;
    Direction moving_toward_;
    ugdk::Vector2D last_stable_position_;

};

}

#endif /* HORUSEYE_GAME_SPRITE_BLOCK_H_ */
