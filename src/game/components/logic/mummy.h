#ifndef HORUSEYE_COMPONENT_LOGIC_MUMMY_H_
#define HORUSEYE_COMPONENT_LOGIC_MUMMY_H_

#include <queue>
#include <ugdk/time.h>
#include <ugdk/math/vector2D.h>

#include "game/components/logic/creature.h"

namespace component {

class Mummy : public Creature {
  
  public:
    Mummy(sprite::WorldObject* owner, double time_to_think = -1);
    ~Mummy();
    
    void set_standing(bool standing) { standing_ = standing; }	
    void set_speed(double speed) { original_speed_ = speed_ = speed; }
    void set_bound(double radius);
    void OnWorldAdd(scene::World* world);

  protected:
    ugdk::time::TimeAccumulator *interval_;
    double time_to_think_, starting_time_to_think_;
    bool standing_;
    ugdk::Vector2D last_direction_;
    std::queue<ugdk::Vector2D> path_;

    virtual void Update(double delta_t);
    virtual void Think(double dt);
    void UpdateDirection(ugdk::Vector2D destination);
    void RandomMovement();

    void MummyAntiStack(sprite::WorldObject *);
    friend class MummyAntiStackCollision;
};

}
#endif // HORUSEYE_COMPONENT_LOGIC_MUMMY_H_
