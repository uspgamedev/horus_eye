#ifndef PYRAMIDWORKS_COLLISION_COLLISIONLOGIC_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONLOGIC_H_

namespace pyramidworks {
namespace collision {

#define COLLISION_DIRECT(DATA, NAME, ARG) class NAME : public pyramidworks::collision::CollisionLogic { \
	protected:	DATA data_; \
	public:		NAME ( DATA data) : data_(data) {} \
	void Handle(void*); }; \
    void NAME::Handle(void* ARG)

class CollisionLogic {
  public:
    CollisionLogic() {}
    virtual ~CollisionLogic() {}

    virtual void Handle(void *) = 0;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONLOGIC_H_ */
