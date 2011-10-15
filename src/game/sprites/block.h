#ifndef HORUSEYE_GAME_SPRITE_BLOCK_H_
#define HORUSEYE_GAME_SPRITE_BLOCK_H_

#include "game/sprites/worldobject.h"

namespace ugdk {
class Image;
}

namespace sprite {

class Projectile;
class Block : public WorldObject {
  public:
    Block(ugdk::Image* image);
    ~Block();

    virtual void Update(float dt);
    virtual void HandleCollision(WorldObject *);

    virtual void CollidesWith(Projectile * obj);

    enum Direction { LEFT, RIGHT, UP, DOWN };

  private:
    bool moving_;
    float moving_time_left_;
    Direction moving_toward_;
};

}

#endif /* HORUSEYE_GAME_SPRITE_BLOCK_H_ */
