//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/wall.h
// Definicao da classe Wall
//

#ifndef HORUSEYE_GAME_SPRITE_WALL_H_
#define HORUSEYE_GAME_SPRITE_WALL_H_

#include "../../framework/sprite.h"
#include "../../framework/engine.h"
#include "worldobject.h"
#include "../../framework/animation.h"

namespace sprite {

class Wall : public WorldObject {

  public:
    enum WallType { MIDDLE, RIGHT, BOTTOM, BOTTOMRIGHT };

    Wall();
    ~Wall();
    virtual void Update(float dt);
    virtual void HandleCollision(WorldObject *);
    void set_type(WallType);

    WallType wall_type_;

  private:

    class Square {
      public:
        framework::Vector2D top_left_, bot_right_;
        Square() {}
        Square(framework::Vector2D& top_left, framework::Vector2D& bot_right) {
            top_left_ = top_left;
            bot_right_ = bot_right;
        }

        bool Contains(framework::Vector2D pos) {
            return (top_left_.x <= pos.x && pos.x <= bot_right_.x)
                    && (top_left_.y <= pos.y && pos.y <= bot_right_.y);
        }
    };

    Square transparency_square_;

    framework::Animation *visible_animation_, *transparent_animation_;
};

}

#endif /* WALL_H_ */
