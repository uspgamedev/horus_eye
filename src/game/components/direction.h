#ifndef HORUSEYE_COMPONENT_DIRECTION_H_
#define HORUSEYE_COMPONENT_DIRECTION_H_

#include <ugdk/math/vector2D.h>

namespace component {

class Direction {
  public:
    Direction() : direction_(0) {}
    static const Direction Right() { return Direction(RIGHT); }
    static const Direction  Left() { return Direction(LEFT);  }
    static const Direction    Up() { return Direction(UP);    }
    static const Direction  Down() { return Direction(DOWN);  }

    static Direction FromScreenVector(const ugdk::math::Vector2D& versor);
    
    static Direction FromWorldVector(const ugdk::math::Vector2D& versor);

    Direction operator|= (const Direction& rhs) {
        direction_ |= rhs.direction_;
        return *this;
    }

    const Direction operator| (const Direction& rhs) const {
        return Direction(direction_ | rhs.direction_);
    }

    /** @return True, if both objects contains at least one matching direction. */
    bool operator& (const Direction& rhs) const {
        return !!(direction_ & rhs.direction_);
    }

    /** @return True, if the object matches the none direction. */
    bool operator!() const { return !direction_; }

    operator const bool() const { return !!direction_; }

    bool operator== (const Direction& rhs) const { return direction_ == rhs.direction_; }

    int value() const { return direction_; }

    int NumDirections() const {
        // Count how many bits 1 there is in direction_
        int result = 0;
        for(int i = direction_; i > 0; i <<= 1)
            if(i & 1) result += 1;
        return result;
    }

    ugdk::math::Vector2D ToVector2D() const {
        ugdk::math::Vector2D dir (0, 0);
        if(direction_ & UP)    dir += ugdk::math::Vector2D( 1,  1);
        if(direction_ & DOWN)  dir += ugdk::math::Vector2D(-1, -1);
        if(direction_ & LEFT)  dir += ugdk::math::Vector2D(-1,  1);
        if(direction_ & RIGHT) dir += ugdk::math::Vector2D( 1, -1);
        return ugdk::math::Vector2D::Normalized(dir);
    }

  private:
    Direction(int direction) : direction_(direction) {}

    int direction_;

    static const int RIGHT = 1;
    static const int LEFT = 2;
    static const int UP = 4;
    static const int DOWN = 8;
};

}  // namespace component

#endif  // HORUSEYE_COMPONENT_DIRECTION_H_
