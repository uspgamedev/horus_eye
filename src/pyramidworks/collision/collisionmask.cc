#include "collisionmask.h"
#include "pyramidworks/collision/collisionobject.h"

namespace pyramidworks {
namespace collision {

CollisionMask* CollisionMaskManager::Get(std::string &name) {
	CollisionMask *mask = cache_[name];
	if(mask == NULL) cache_[name] = mask = new CollisionMask(name, NULL);
	return mask;
}

const CollisionMask* CollisionMaskManager::Generate(std::string name, std::string parent) {
	CollisionMask *parent_mask = cache_[parent];
	if(parent_mask == NULL) parent_mask = Get(parent);

	CollisionMask *mask = cache_[name];
	if(mask == NULL) cache_[name] = mask = new CollisionMask(name, parent_mask);
	else mask->set_parent(parent_mask);

	return mask;
}

void CollisionMask::AddObject(CollisionObject *obj) { 
    objects_.push_front(obj);
}

void CollisionMask::RemoveObject(CollisionObject *obj) { 
    objects_.remove(obj); 
}

CollisionMaskManager *CollisionMaskManager::reference_ = NULL;

} // namespace collision
} // namespace pyramidworks
