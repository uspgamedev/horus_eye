#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_

#include <string>
#include <map>

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

/// A singleton that manages all collisions.
class CollisionManager {
  public:
    /// The singleton's reference method.
    /** @return A pointer to the only CollisionManager. */
	static CollisionManager* reference() {
		return reference_ ? reference_ : reference_ = new CollisionManager;
	}

    /// Returns a CollisionClass of the asked name.
    /** Searches for a CollisionClass with the given name, creating a new one if 
      * none is found.
      * @param name The name to search for.
      * @return A pointer to a CollisionClass. */
	CollisionClass* Generate(std::string name) { return Get(name); }
	const CollisionClass* Generate(std::string name, std::string parent);

	~CollisionManager();

  private:
    // Singleton stuff
	CollisionManager() {}
    static CollisionManager *reference_;

    CollisionClass* Get(std::string &name);

	std::map<std::string, CollisionClass*> cache_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_ */
