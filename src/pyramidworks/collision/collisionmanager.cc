#include "collisionmanager.h"

#include "pyramidworks/collision/collisionmask.h"
#include "pyramidworks/collision/collisionobject.h"

namespace pyramidworks {
namespace collision {

CollisionManager::~CollisionManager() { 
    reference_ = NULL;
    std::map<std::string, CollisionMask*>::iterator it;
    for(it = cache_.begin(); it != cache_.end(); ++it) {
        delete it->second;
    }
    cache_.clear();
}

CollisionMask* CollisionManager::Get(std::string &name) {
	CollisionMask *mask = cache_[name];
	if(mask == NULL) {
        cache_[name] = mask = new CollisionMask();
#ifdef DEBUG
        mask->set_name(name);
#endif
    }
	return mask;
}

const CollisionMask* CollisionManager::Generate(std::string name, std::string parent) {
	CollisionMask *parent_mask = Get(parent);
	CollisionMask *mask = Get(name);
    if(parent_mask != NULL)
        mask->set_parent(parent_mask);
	return mask;
}

CollisionManager *CollisionManager::reference_ = NULL;

} // namespace collision
} // namespace pyramidworks
