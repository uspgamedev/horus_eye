#include "collisionmask.h"

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

CollisionMaskManager *CollisionMaskManager::reference_ = NULL;

} // namespace collision
} // namespace pyramidworks
