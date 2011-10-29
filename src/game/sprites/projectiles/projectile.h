#ifndef HORUSEYE_GAME_SPRITES_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_PROJECTILE_H_

#include "game/sprites/worldobject.h"
#include <ugdk/math/vector2D.h>

namespace ugdk {
class TimeAccumulator;
class Image;
}

namespace sprite {

class Projectile : public WorldObject {
  
  public:
    Projectile(int damage, float speed, int duration, ugdk::Vector2D &dir);
    virtual ~Projectile();
    void Move(float delta_t);
    void Update(float delta_t); 

    int damage() { return damage_; }

    virtual void Explode();

  protected:
    int damage_;
    float speed_;
    ugdk::Vector2D direction_;
    ugdk::TimeAccumulator *duration_;
    bool exploding_;
};

}

#endif /* HORUSEYE_GAME_SPRITES_PROJECTILE_H_ */
