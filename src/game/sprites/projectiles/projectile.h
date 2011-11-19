#ifndef HORUSEYE_GAME_SPRITES_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_PROJECTILE_H_

#include "game/sprites/timedworldobject.h"
#include <ugdk/math/vector2D.h>

namespace sprite {

class Projectile : public TimedWorldObject {
  private: typedef TimedWorldObject super;
  public:
    Projectile(int damage, float speed, int duration, ugdk::Vector2D &dir);
    void Move(float delta_t);
    void Update(float delta_t);

    void set_collision_object(CollisionObject* col) { collision_object_ = col; }
    int damage() const { return damage_; }

  protected:
    int damage_;
    float speed_;
    ugdk::Vector2D direction_;
};

}

#endif /* HORUSEYE_GAME_SPRITES_PROJECTILE_H_ */
