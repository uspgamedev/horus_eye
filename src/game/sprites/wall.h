#ifndef HORUSEYE_GAME_SPRITE_WALL_H_
#define HORUSEYE_GAME_SPRITE_WALL_H_

#include "worldobject.h"

namespace ugdk {
class Animation;
class Image;
}

namespace utils {
class Tile;
}

namespace sprite {

class Wall : public WorldObject {

  public:
    enum WallType { MIDDLE, RIGHT, BOTTOM, BOTTOMRIGHT };

    Wall(ugdk::Image* image);
    ~Wall();
    virtual void Update(float dt);

    
    
    void set_type(WallType);

    WallType wall_type_;

    static const CollisionMask Collision() { return collision_; }
    virtual const CollisionMask collision() const { return Wall::Collision(); }

  private:
    static const CollisionMask collision_;

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

    Square transparency_square_;

    utils::Tile *tile_;
    int     visible_frame_,
            transparent_frame_,
            dark_visible_frame_,
            dark_transparent_frame_;
};

}

#endif /* WALL_H_ */
