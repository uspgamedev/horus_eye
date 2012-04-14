#ifndef HORUSEYE_GAME_SPRITE_MUMMY_H_
#define HORUSEYE_GAME_SPRITE_MUMMY_H_

#include <queue>
#include <ugdk/math/vector2D.h>

#include "game/components/creature.h"

namespace ugdk {
    namespace graphic {
        class FlexibleSpritesheet;
    }
    namespace time {
        class TimeAccumulator;
    }
}

#define TIME_TO_THINK 0.1

namespace component {

class Mummy : public Creature {
  
  public:
    Mummy(sprite::WorldObject* owner, ugdk::graphic::FlexibleSpritesheet* img);
    ~Mummy();
    
    void set_standing(bool standing) { standing_ = standing; }	
    void set_speed(double speed) { original_speed_ = speed_ = speed; }
    //void TakeDamage(double life_points);
    void set_weapon(skills::Skill *weapon) { weapon_ = weapon; }
    void set_bound(double radius);

    void StartAttack(sprite::WorldObject* obj);
    
  protected:
    ugdk::time::TimeAccumulator *interval_;
    double time_to_think_;
    bool standing_;
    ugdk::Vector2D last_direction_;
    skills::Skill *weapon_;
    std::queue<ugdk::Vector2D> path_;

    virtual void Update(double delta_t);
    virtual void Think(double dt);
    void UpdateDirection(ugdk::Vector2D destination);
    void RandomMovement();
    void PlayHitSound() const;

    void MummyAntiStack(sprite::WorldObject *);
    friend class MummyAntiStackCollision;
};

}
#endif // MUMMY_H_