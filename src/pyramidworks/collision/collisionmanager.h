#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_

#include <string>
#include <map>
#include <list>

namespace pyramidworks {
namespace collision {

#define INITIALIZE_COLLIDABLE_ROOT(CLASS) \
    static const pyramidworks::collision::CollisionClass* CollisionMaskInitializer = \
    pyramidworks::collision::CollisionManager::reference()->Generate( #CLASS );

#define INITIALIZE_COLLIDABLE_NODE(CLASS, PARENT_CLASS) \
    static const pyramidworks::collision::CollisionClass* CollisionMaskInitializer = \
    pyramidworks::collision::CollisionManager::reference()->Generate( #CLASS , #PARENT_CLASS );

#define GET_COLLISIONMASK(NAME) pyramidworks::collision::CollisionManager::reference()->Generate( #NAME )


class CollisionClass;
class CollisionManager {
  public:
	static CollisionManager* reference() {
		return reference_ ? reference_ : reference_ = new CollisionManager;
	}

	CollisionClass* Generate(std::string name) { return Get(name); }
	const CollisionClass* Generate(std::string name, std::string parent);

	~CollisionManager();

  private:
    /// Singleton stuff
	CollisionManager() {}
    static CollisionManager *reference_;

    CollisionClass* Get(std::string &name);

	std::map<std::string, CollisionClass*> cache_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_ */
