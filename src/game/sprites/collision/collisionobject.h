#ifndef HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONOBJECT_H_
#define HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONOBJECT_H_

namespace sprite {

class WorldObject;
class CollisionObject {
  public:
    CollisionObject() {}
    virtual ~CollisionObject() {}

    virtual void Handle(WorldObject *obj) {}
};

}

#endif /* HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONOBJECT_H_ */
