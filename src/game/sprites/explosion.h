#ifndef HORUSEYE_GAME_SPRITES_EXPLOSION_H_
#define HORUSEYE_GAME_SPRITES_EXPLOSION_H_

#include "worldobject.h"
#include "../../framework/vector2D.h"
#include "../../framework/observer.h"

namespace framework {
class TimeAccumulator;
}

using namespace framework;

namespace sprite {

class Mummy;
class Explosion : public WorldObject, Observer {

  public:
	Explosion(Image *img, Animation *, float radius, float damage);
	~Explosion();
	void RadiusUpdate(float delta_t);
	void Update(float delta_t);
    virtual void Tick();

	void CollidesWith(Mummy *);
	virtual void HandleCollision(WorldObject *);
	int damage() { return damage_; }

  private:
	int damage_;
    float radius_;
    float expansion_speed_;
    Vector2D direction_;
};

}

#endif /* HORUSEYE_GAME_SPRITES_EXPLOSION_H_ */
