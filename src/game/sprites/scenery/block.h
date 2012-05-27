#ifndef HORUSEYE_GAME_SPRITE_BLOCK_H_
#define HORUSEYE_GAME_SPRITE_BLOCK_H_

#include <ugdk/math/vector2D.h>
#include "game/sprites/worldobject.h"

namespace sprite {

class Block : public WorldObject {
  
  public:
    Block(ugdk::graphic::Spritesheet* image);
    ~Block();
    virtual void Update(double dt);
    
    enum Direction { LEFT, RIGHT, UP, DOWN };

  private:
#ifdef DEBUG
    void GetKeys();
#endif
    void MoveBlock(double dt);
    
    void RevertPosition();
    void PushToward(ugdk::Vector2D &pushdir);

    bool moving_;
    double moving_time_left_;
    Direction moving_toward_;
    ugdk::Vector2D last_stable_position_;

    friend class InvalidMovementCollision;
    friend class PushOnCollision;
};

}

#endif /* HORUSEYE_GAME_SPRITE_BLOCK_H_ */
