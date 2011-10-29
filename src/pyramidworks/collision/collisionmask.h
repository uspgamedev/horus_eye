#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_

#include <string>
#include <map>
#include <list>

namespace pyramidworks {
namespace collision {

#define DEFINE_COLLIDABLE public: \
    static const pyramidworks::collision::CollisionMask* Collision() { return collision_; } \
    virtual const pyramidworks::collision::CollisionMask* collision() const { return Collision(); } \
  private: \
    static const pyramidworks::collision::CollisionMask* collision_;

#define INITIALIZE_COLLIDABLE_ROOT(CLASS) \
    const pyramidworks::collision::CollisionMask* CLASS::collision_ = \
    pyramidworks::collision::CollisionMaskManager::reference()->Generate( #CLASS );

#define INITIALIZE_COLLIDABLE_NODE(CLASS, PARENT_CLASS) \
    const pyramidworks::collision::CollisionMask* CLASS::collision_ = \
    pyramidworks::collision::CollisionMaskManager::reference()->Generate( #CLASS , #PARENT_CLASS );

#define GET_COLLISIONMASK(NAME) pyramidworks::collision::CollisionMaskManager::reference()->Get( #NAME )


class CollisionObject;
class CollisionMask;
class CollisionMaskManager {
  public:
	static CollisionMaskManager* reference() {
		return reference_ ? reference_ : reference_ = new CollisionMaskManager;
	}

	CollisionMask* Get(std::string &name);
	const CollisionMask* Generate(std::string name) { return Get(name); }
	const CollisionMask* Generate(std::string name, std::string parent);

	~CollisionMaskManager() { reference_ = NULL; }

  private:
	CollisionMaskManager() {}
	std::map<std::string, CollisionMask*> cache_;
	static CollisionMaskManager *reference_;
};

class CollisionMask {
  public:
    ~CollisionMask() {}
    const CollisionMask* parent() const { return parent_; }
	void set_parent(CollisionMask* parent) { parent_ = parent; }


    void AddObject(CollisionObject *obj);
    void RemoveObject(CollisionObject *obj);

  private:
	friend class CollisionMaskManager;
    CollisionMask(std::string name, const CollisionMask* parent = NULL) : name_(name), parent_(parent) {}

	// Unnecessary, used for debugging purposes.
	std::string name_;

    const CollisionMask* parent_;
    std::list<CollisionObject *> objects_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_ */
