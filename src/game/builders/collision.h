#ifndef HORUSEYE_BUILDERS_COLLISION_H_
#define HORUSEYE_BUILDERS_COLLISION_H_

#include <pyramidworks/collision/collisionlogic.h>
#include "game/constants.h"
#include "game/sprites.h"

namespace builder {

class DieCollision : public pyramidworks::collision::CollisionLogic {
  public:
    DieCollision(sprite::WorldObject* owner)
        : owner_(owner) {}

    void Handle(void* obj);

  private:
    sprite::WorldObject* owner_;
};

class BounceCollision : public pyramidworks::collision::CollisionLogic {
  public:
    BounceCollision(sprite::WorldObject* owner)
        : owner_(owner) {}

    void Handle(void* obj);

  private:
    sprite::WorldObject* owner_;
};

class DamageCollision : public pyramidworks::collision::CollisionLogic {
  public:
    DamageCollision(double damage) : damage_(damage) {}
    DamageCollision(const std::string& constant_name) : damage_(constants::GetDouble(constant_name)) {}

    void Handle(void* obj);

  private:
    double damage_;
};

class DamageAndDieCollision : public pyramidworks::collision::CollisionLogic {
  public:
    DamageAndDieCollision(sprite::WorldObject* owner, double damage)
        :   owner_(owner), 
            damage_(damage) {}

    DamageAndDieCollision(sprite::WorldObject* owner, const std::string& constant_name) 
        :   owner_(owner),
            damage_(constants::GetDouble(constant_name)) {}

    void Handle(void* obj);

  private:
    sprite::WorldObject* owner_;
    double damage_;
};

} // namespace builder

#endif // HORUSEYE_BUILDERS_COLLISION_H_