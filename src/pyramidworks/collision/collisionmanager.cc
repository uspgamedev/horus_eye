#include "collisionmanager.h"

#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/collision/collisionobject.h"

namespace pyramidworks {
namespace collision {

CollisionManager::~CollisionManager() { 
    reference_ = NULL;
    std::map<std::string, CollisionClass*>::iterator it;
    for(it = cache_.begin(); it != cache_.end(); ++it) {
        delete it->second;
    }
    cache_.clear();
}

CollisionClass* CollisionManager::Get(std::string &name) {
	CollisionClass *mask = cache_[name];
	if(mask == NULL) {
        cache_[name] = mask = new CollisionClass();
#ifdef DEBUG
        mask->set_name(name);
#endif
    }
	return mask;
}

const CollisionClass* CollisionManager::Generate(std::string name, std::string parent) {
	CollisionClass *parent_mask = Get(parent);
	CollisionClass *mask = Get(name);
    if(parent_mask != NULL)
        mask->set_parent(parent_mask);
	return mask;
}

CollisionManager *CollisionManager::reference_ = NULL;

} // namespace collision
} // namespace pyramidworks
