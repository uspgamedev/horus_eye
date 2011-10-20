#ifndef HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONOBJECT_H_
#define HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONOBJECT_H_

namespace sprite {

#define COLLISION_BEGIN struct Collisions {
#define COLLISION_END	};
#define COLLISION_ADD(OWNER, NAME) class NAME : public CollisionObject { \
	protected:	OWNER *owner_; \
	public:		NAME ( OWNER *onwer) : owner_(onwer) {} \
	void Handle(WorldObject* obj); };

#define COLLISION_ADD_INLINE(OWNER, NAME, CODE) class NAME : public CollisionObject { \
	protected:	OWNER *owner_; \
	public:		NAME ( OWNER *onwer) : owner_(onwer) {} \
	void Handle(WorldObject* obj) { CODE } };

#define COLLISION_IMPLEMENT(ONWER, NAME, DATA) void ONWER::Collisions::NAME::Handle(WorldObject *DATA)

class WorldObject;
class CollisionObject {
  public:
    CollisionObject() {}
    virtual ~CollisionObject() {}

    virtual void Handle(WorldObject *obj) {}
};

}

#endif /* HORUSEYE_GAME_SPRITE_COLLISION_COLLISIONOBJECT_H_ */
