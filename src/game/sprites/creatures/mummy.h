#ifndef HORUSEYE_GAME_SPRITE_MUMMY_H_
#define HORUSEYE_GAME_SPRITE_MUMMY_H_

#include <queue>
#include <ugdk/math/vector2D.h>

#include "game/sprites/creatures/creature.h"
#include "game/skills/skill.h"

#define TIME_TO_THINK 0.1f

namespace ugdk {
class TimeAccumulator;
class FlexibleSpritesheet;
}

namespace sprite {

class Mummy : public Creature {
  
  public:
    Mummy(ugdk::FlexibleSpritesheet* img);
    ~Mummy();

    
    void set_standing(bool standing) { standing_ = standing; }	
    void set_speed(float speed) { original_speed_ = speed_ = speed; }
    void TakeDamage(float life_points);
	void set_weapon(skills::Skill *weapon) { weapon_ = weapon; }
    void set_bound(float radius);

    void StartAttack(Creature* obj);
    
  protected:
    ugdk::TimeAccumulator *interval_;
    float time_to_think_;
    bool standing_;
    Vector2D last_direction_;
	skills::Skill *weapon_;
    std::queue<Vector2D> path_;

    virtual void Update(float delta_t);
    virtual void Think(float dt);
    void UpdateDirection(Vector2D destination);
    void RandomMovement();
    void StartToDie();
    void PlayHitSound() const;

    void MummyAntiStack(WorldObject *);
    friend class MummyAntiStackCollision;
};

}
#endif // MUMMY_H_
