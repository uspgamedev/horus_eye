#ifndef HORUSEYE_GAME_ENTITIES_EMITTABLE_H_
#define HORUSEYE_GAME_ENTITIES_EMITTABLE_H_

namespace entity {

// Note : doesn't extend WorldObject, since WorldObject is not an interface.

class Emittable {
  public:
    virtual void Emit() = 0;
};

} // namespace entity

#endif