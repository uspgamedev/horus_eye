#ifndef HORUSEYE_COMPONENT_LOGIC_WALL_H_
#define HORUSEYE_COMPONENT_LOGIC_WALL_H_

#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include "game/components/logic.h"
#include "game/sprites.h"
#include "game/utils/tilefwd.h"

namespace component {

class Wall : public Logic {
  public:
    enum WallType { MIDDLE, RIGHT, BOTTOM, BOTTOMRIGHT };

    Wall(sprite::WorldObject* owner, ugdk::graphic::Spritesheet* sheet);
    ~Wall();

    void Update(double dt);
    
    void set_type(WallType);
    WallType wall_type_;

  private:
    void CheckType();

    class Square {
      public:
        ugdk::Vector2D top_left_, bot_right_;
        Square() {}
        Square(ugdk::Vector2D& top_left, ugdk::Vector2D& bot_right) {
            top_left_ = top_left;
            bot_right_ = bot_right;
        }

        bool Contains(ugdk::Vector2D pos) {
            return (top_left_.x <= pos.x && pos.x <= bot_right_.x)
                    && (top_left_.y <= pos.y && pos.y <= bot_right_.y);
        }
    };

    sprite::WorldObject* owner_;
    Square transparency_square_;
    ugdk::graphic::Sprite* sprite_;

    utils::Tile *tile_;
    int     visible_frame_,
            transparent_frame_,
            dark_visible_frame_,
            dark_transparent_frame_;
};

}

#endif /* HORUSEYE_COMPONENT_LOGIC_WALL_H_ */
