#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_

#include <string>
#include <map>
#include <list>

namespace pyramidworks {
namespace collision {

#define INITIALIZE_COLLIDABLE_ROOT(CLASS) \
    static const pyramidworks::collision::CollisionMask* CollisionMaskInitializer = \
    pyramidworks::collision::CollisionManager::reference()->Generate( #CLASS );

#define INITIALIZE_COLLIDABLE_NODE(CLASS, PARENT_CLASS) \
    static const pyramidworks::collision::CollisionMask* CollisionMaskInitializer = \
    pyramidworks::collision::CollisionManager::reference()->Generate( #CLASS , #PARENT_CLASS );

#define GET_COLLISIONMASK(NAME) pyramidworks::collision::CollisionManager::reference()->Generate( #NAME )


class CollisionMask;
class CollisionManager {
  public:
	static CollisionManager* reference() {
		return reference_ ? reference_ : reference_ = new CollisionManager;
	}

	CollisionMask* Get(std::string &name);
	CollisionMask* Generate(std::string name) { return Get(name); }
	const CollisionMask* Generate(std::string name, std::string parent);

	~CollisionManager();

  private:
	CollisionManager() {}
    static CollisionManager *reference_;

	std::map<std::string, CollisionMask*> cache_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_ */
