#ifndef HORUSEYE_GAME_SPRITE_MUMMY_H_
#define HORUSEYE_GAME_SPRITE_MUMMY_H_

#include <queue>
#include <ugdk/math/vector2D.h>
#include "game/sprites/creatures/creature.h"
#include "game/utils/circleobject.h"

#define TIME_TO_THINK 0.1f

namespace ugdk {
class TimeAccumulator;
class Image;
}

namespace sprite {

class Weapon;

class Mummy : public Creature {
  DEFINE_COLLIDABLE
  public:
    Mummy(ugdk::Image* img);
    ~Mummy();

    
    void set_standing(bool standing) { standing_ = standing; }	
    void set_speed(float speed) { original_speed_ = speed_ = speed; }
    void TakeDamage(float life_points);
	void set_weapon(Weapon *weapon) { weapon_ = weapon; }
    void set_bound(float radius) {
		if(bound_ != NULL) delete bound_;
		bound_ = new utils::CircleObject(radius);
	}

    void StartAttack(Creature* obj);
  protected:
    ugdk::TimeAccumulator *interval_;
    float time_to_think_;
    bool standing_;
    Vector2D last_direction_;
	Weapon *weapon_;
    std::queue<Vector2D> path_;

	void MummyAntiStack(WorldObject *);

	COLLISION_BEGIN
        COLLISION_ADD_INLINE (Mummy, MummyAntiStack, WorldObject *obj = (WorldObject *) data; owner_->MummyAntiStack(obj); )
	COLLISION_END

    virtual void Update(float delta_t);
    virtual void Think(float dt);
    void UpdateDirection(Vector2D destiny);
    void RandomMovement();
    void Die();
    void PlayHitSound() const;
};

}
#endif // MUMMY_H_
