#ifndef HORUSEYE_COMPONENT_LOGIC_CREATURE_H_
#define HORUSEYE_COMPONENT_LOGIC_CREATURE_H_

#include <list>
#include <map>

#include <ugdk/action.h>
#include <ugdk/time.h>
#include <ugdk/graphic.h>

#include <ugdk/action/observer.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>
#include <ugdk/util/uncopyable.h>
#include <pyramidworks/geometry.h>
#include <pyramidworks/collision.h>

#include "game/components.h"
#include "game/sprites.h"
#include "game/skills.h"

#include "game/components/logic.h"
#include "game/components/direction.h"
#include "game/resources/energy.h"
#include "game/skills/usearguments.h"

namespace component {
    
class Creature : public Logic, public ugdk::util::Uncopyable { 
  public:
    Creature(sprite::WorldObject* owner, double speed = 0.0);
    virtual ~Creature();

    sprite::WorldObject* owner() { return owner_; }

  protected:
    friend class RectCollision;

    virtual void AddKnownCollisions();

    virtual void Update(double dt);

    void UseSkills();
    void StartAttackAnimation();
    void Move(ugdk::Vector2D direction, double delta_t);
    void Move(ugdk::Vector2D distance);
    
    ugdk::Vector2D GetWalkingDirection() { return walking_direction_; }
    void CollideWithRect(const pyramidworks::collision::CollisionObject*);


    /// The owner.
    sprite::WorldObject* owner_;
    
    ///
    Direction animation_direction_;

    /// 
    Direction last_standing_direction_;

    /// The last position this creature was that is guaranteed to not colide with any walls.
    ugdk::Vector2D last_stable_position_;

    double last_dt_;

    /// How fast this creature moves per second.
    double speed_;

    /// Stores the original speed, so one can alter the speed temporarily.
    double original_speed_;

    /// The direction this creature is moving to.
    ugdk::Vector2D walking_direction_;

};  // class Creature

}  // namespace sprite

#endif  // HORUSEYE_COMPONENT_LOGIC_CREATURE_H_
