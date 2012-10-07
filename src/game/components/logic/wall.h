#ifndef HORUSEYE_COMPONENT_LOGIC_WALL_H_
#define HORUSEYE_COMPONENT_LOGIC_WALL_H_

#include "game/components/base.h"

#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include "game/sprites.h"
#include "game/map.h"

namespace component {

class Wall : public Base {
  public:
    enum WallType { MIDDLE, RIGHT, BOTTOM, BOTTOMRIGHT };
    static const char* DEFAULT_NAME() { return "wall"; }

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

    map::Tile *tile_;
    int     visible_frame_,
            transparent_frame_,
            dark_visible_frame_,
            dark_transparent_frame_;
};

}

#endif /* HORUSEYE_COMPONENT_LOGIC_WALL_H_ */
