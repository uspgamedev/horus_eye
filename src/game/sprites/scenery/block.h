#ifndef HORUSEYE_GAME_SPRITE_BLOCK_H_
#define HORUSEYE_GAME_SPRITE_BLOCK_H_

#include <ugdk/math/vector2D.h>
#include "game/sprites/worldobject.h"

namespace ugdk {
class Image;
}

namespace sprite {

class Block : public WorldObject {
  
  public:
    Block(ugdk::FlexibleSpritesheet* image);
    ~Block();
    virtual void Update(float dt);
    
    enum Direction { LEFT, RIGHT, UP, DOWN };

  private:
#ifdef DEBUG
    void GetKeys();
#endif
    void MoveBlock(float dt);
    
    void RevertPosition();
    void PushToward(ugdk::Vector2D &pushdir);

    bool moving_;
    float moving_time_left_;
    Direction moving_toward_;
    ugdk::Vector2D last_stable_position_;

    friend class InvalidMovementCollision;
    friend class PushOnCollision;
};

}

#endif /* HORUSEYE_GAME_SPRITE_BLOCK_H_ */
