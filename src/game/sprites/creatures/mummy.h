#ifndef HORUSEYE_GAME_SPRITE_MUMMY_H_
#define HORUSEYE_GAME_SPRITE_MUMMY_H_

#include <ugdk/math/vector2D.h>

#include "game/sprites/creatures/creature.h"
#include "game/skills/skill.h"

namespace ugdk {
class Image;
}

namespace sprite {

class Mummy : public Creature {
  
  public:
    Mummy(ugdk::Image* img);
    ~Mummy();

	bool get_standing() { return standing_; }
    void set_standing(bool standing) { standing_ = standing; }	
    void set_speed(float speed) { original_speed_ = speed_ = speed; }
    void TakeDamage(float life_points);
	skills::Skill* get_weapon() { return weapon_; }
	void set_weapon(skills::Skill *weapon) { weapon_ = weapon; }
    void set_bound(float radius);

	void set_last_known_hero_pos(Vector2D pos) { last_known_hero_pos_ = pos; }
	Vector2D get_last_known_hero_pos() { return last_known_hero_pos_; }

	void UpdateDirections(Vector2D target_pos);

    void StartAttack(Creature* obj);
    
  protected:
    bool standing_;
	skills::Skill *weapon_;
	Vector2D last_known_hero_pos_;

    virtual void Update(float delta_t);
    void StartToDie();
    void PlayHitSound() const;

    void MummyAntiStack(WorldObject *);
    friend class MummyAntiStackCollision;
};

}
#endif // MUMMY_H_
