#ifndef HORUSEYE_GAME_SPRITES_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_PROJECTILE_H_

#include "worldobject.h"
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
    struct Collisions {
        class Explode : public CollisionObject {
          public:
            Explode(Projectile* onwer) : owner_(onwer) {}
			void Handle(WorldObject* obj) { owner_->Explode(); }

          protected:
            Projectile *owner_;
        };
        class Damage : public CollisionObject {
          public:
            Damage(Projectile* onwer) : owner_(onwer) {}
			void Handle(WorldObject* obj);

          protected:
            Projectile *owner_;
        };
    };
};

}

#endif /* HORUSEYE_GAME_SPRITES_PROJECTILE_H_ */
