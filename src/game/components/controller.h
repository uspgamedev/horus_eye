#ifndef HORUSEYE_COMPONENT_CONTROLLER_H_
#define HORUSEYE_COMPONENT_CONTROLLER_H_

#include <ugdk/math/vector2D.h>
#include "game/sprites.h"

namespace component {

class Direction {
  public:
    Direction() : direction_(0) {}
    static const Direction Right() { return Direction(RIGHT); }
    static const Direction  Left() { return Direction(LEFT);  }
    static const Direction    Up() { return Direction(UP);    }
    static const Direction  Down() { return Direction(DOWN);  }

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

    ugdk::Vector2D ToVector2D() const {
        ugdk::Vector2D dir (0, 0);
        if(direction_ & UP)    dir += ugdk::Vector2D( 1,  1);
        if(direction_ & DOWN)  dir += ugdk::Vector2D(-1, -1);
        if(direction_ & LEFT)  dir += ugdk::Vector2D(-1,  1);
        if(direction_ & RIGHT) dir += ugdk::Vector2D( 1, -1);
        return ugdk::Vector2D::Normalized(dir);
    }

  private:
    Direction(int direction) : direction_(direction) {}

    int direction_;

    static const int RIGHT = 1;
    static const int LEFT = 2;
    static const int UP = 4;
    static const int DOWN = 8;
};

class Controller { 
  public:
    enum WeaponSlot {
        PRIMARY, SECONDARY, SPECIAL1, SPECIAL2, SPECIAL3
    };

    Controller(sprite::WorldObject* owner) : owner_(owner) {}
    virtual ~Controller() {}

          sprite::WorldObject* owner()       { return owner_; }
    const sprite::WorldObject* owner() const { return owner_; }

    virtual void Update(double dt) = 0;

    virtual bool IsUsingWeaponSlot(WeaponSlot) const = 0;

    virtual const ugdk::Vector2D& direction_vector() const = 0;
    virtual const Direction& direction() const { return dir_; }

  protected:
    Direction dir_;

    /// The owner.
    sprite::WorldObject* owner_;

};  // class Controller

}  // namespace component

#endif  // HORUSEYE_COMPONENT_CONTROLLER_H_
