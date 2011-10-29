#ifndef PYRAMIDWORKS_COLLISION_COLLISIONLOGIC_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONLOGIC_H_

namespace pyramidworks {
namespace collision {

#define COLLISION_BEGIN struct Collisions {
#define COLLISION_END	};
#define COLLISION_ADD(OWNER, NAME) class NAME : public pyramidworks::collision::CollisionLogic { \
	protected:	OWNER *owner_; \
	public:		NAME ( OWNER *onwer) : owner_(onwer) {} \
	void Handle(void* data); }; \
    friend class NAME;

#define COLLISION_ADD_INLINE(OWNER, NAME, CODE) class NAME : public pyramidworks::collision::CollisionLogic { \
	protected:	OWNER *owner_; \
	public:		NAME ( OWNER *onwer) : owner_(onwer) {} \
	void Handle(void* data) { CODE } }; \
    friend class NAME;

#define COLLISION_IMPLEMENT(ONWER, NAME, DATA) void ONWER::Collisions::NAME::Handle(void *DATA)

class CollisionLogic {
  public:
    CollisionLogic() {}
    virtual ~CollisionLogic() {}

    virtual void Handle(void *data) {}
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONLOGIC_H_ */
