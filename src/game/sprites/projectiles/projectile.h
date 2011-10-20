#ifndef HORUSEYE_GAME_SPRITES_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_PROJECTILE_H_

#include "game/sprites/worldobject.h"
#include <ugdk/math/vector2D.h>

namespace ugdk {
class TimeAccumulator;
}

namespace sprite {

class Projectile : public WorldObject {
  DEFINE_COLLIDABLE
  public:
    Projectile(int damage, float speed, int duration, ugdk::Vector2D &);
    virtual ~Projectile();
    void Move(float delta_t);
    void Update(float delta_t); 

    int damage() { return damage_; }

  protected:
    int damage_;
    float speed_;
    ugdk::Vector2D direction_;
    ugdk::TimeAccumulator *duration_;
    bool exploding_;

	virtual void Explode();
	COLLISION_BEGIN
		COLLISION_ADD_INLINE(Projectile, Explode, owner_->Explode();)
		COLLISION_ADD		(Projectile, Damage)
		COLLISION_ADD		(Projectile, DamageAndExplode)
	COLLISION_END
};

}

#endif /* HORUSEYE_GAME_SPRITES_PROJECTILE_H_ */
