#ifndef PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_

namespace pyramidworks {
namespace collision {

#define COLLISION_BEGIN struct Collisions {
#define COLLISION_END	};
#define COLLISION_ADD(OWNER, NAME) class NAME : public pyramidworks::collision::CollisionObject { \
	protected:	OWNER *owner_; \
	public:		NAME ( OWNER *onwer) : owner_(onwer) {} \
	void Handle(void* data); }; \
    friend class NAME;

#define COLLISION_ADD_INLINE(OWNER, NAME, CODE) class NAME : public pyramidworks::collision::CollisionObject { \
	protected:	OWNER *owner_; \
	public:		NAME ( OWNER *onwer) : owner_(onwer) {} \
	void Handle(void* data) { CODE } }; \
    friend class NAME;

#define COLLISION_IMPLEMENT(ONWER, NAME, DATA) void ONWER::Collisions::NAME::Handle(void *DATA)

class CollisionObject {
  public:
    CollisionObject() {}
    virtual ~CollisionObject() {}

    virtual void Handle(void *data) {}
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_ */
