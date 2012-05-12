#ifndef HORUSEYE_GAME_SPRITES_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_PROJECTILE_H_

#include "game/sprites/worldobject.h"
#include <ugdk/math/vector2D.h>

namespace sprite {

class Projectile : public WorldObject {
  private: typedef WorldObject super;
  public:
    Projectile(double speed, int duration, const ugdk::Vector2D &dir)
    :   WorldObject(duration / 1000.0),
        speed_(speed),
        direction_(dir.Normalize()) {}

    void Move(double delta_t) {
        ugdk::Vector2D velocity = direction_ * (speed_ * delta_t);
        set_world_position(this->world_position() + velocity);
    }

    virtual void Update(double delta_t) {
        super::Update(delta_t);
        if (is_active())
            this->Move(delta_t);
    }

  protected:
    double speed_;
    ugdk::Vector2D direction_;
};

}

#endif /* HORUSEYE_GAME_SPRITES_PROJECTILE_H_ */
