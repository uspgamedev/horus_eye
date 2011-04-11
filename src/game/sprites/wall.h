#ifndef HORUSEYE_GAME_SPRITE_WALL_H_
#define HORUSEYE_GAME_SPRITE_WALL_H_

#include "worldobject.h"

namespace framework {
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

    Wall(framework::Image* image);
    ~Wall();
    virtual void Update(float dt);
    virtual void HandleCollision(WorldObject *);
    void set_type(WallType);

    WallType wall_type_;

  private:

    void CheckType();

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

    utils::Tile *tile_;
    int     visible_frame_,
            transparent_frame_,
            dark_visible_frame_,
            dark_transparent_frame_;
};

}

#endif /* WALL_H_ */
