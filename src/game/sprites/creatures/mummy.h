#ifndef HORUSEYE_GAME_SPRITE_MUMMY_H_
#define HORUSEYE_GAME_SPRITE_MUMMY_H_

#include <ugdk/math/vector2D.h>

#include "game/sprites/creatures/creature.h"

namespace ugdk {
class Image;
}

namespace sprite {

class Mummy : public Creature {
  
  public:
    Mummy(ugdk::Image* img);
    ~Mummy();

	bool standing() { return standing_; }
    void set_standing(bool standing) { standing_ = standing; }	
    void set_speed(float speed) { original_speed_ = speed_ = speed; }
    void TakeDamage(float life_points);
    void set_bound(float radius);

	void set_last_known_hero_pos(Vector2D pos) { last_known_hero_pos_ = pos; }
	Vector2D last_known_hero_pos() { return last_known_hero_pos_; }
	void set_saw_hero(bool saw_hero) { saw_hero_ = saw_hero; }
	bool saw_hero() { return saw_hero_; }

	void UpdateDirections(Vector2D target_pos);

    void StartAttack(Creature* obj);
    
  protected:
    bool standing_;
	Vector2D last_known_hero_pos_;
	bool saw_hero_;

    virtual void Update(float delta_t);
    void StartToDie();
    void PlayHitSound() const;

    void MummyAntiStack(WorldObject *);
    friend class MummyAntiStackCollision;
};

}
#endif // MUMMY_H_
