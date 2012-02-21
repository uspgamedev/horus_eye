#include "collisionmanager.h"

#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/collision/collisionobject.h"

namespace pyramidworks {
namespace collision {

CollisionManager *CollisionManager::reference_ = NULL;

CollisionManager::~CollisionManager() { 
    reference_ = NULL;
    std::map<std::string, CollisionClass*>::iterator it;
    for(it = cache_.begin(); it != cache_.end(); ++it) {
        delete it->second;
    }
    cache_.clear();
}

void CollisionManager::Generate(const std::string &name) {
    cache_[name] = new CollisionClass();
#ifdef DEBUG
    cache_[name]->set_name(name);
#endif
}

void CollisionManager::Generate(const std::string &name, const std::string &parent) {
    CollisionClass* colclass = cache_[name] = new CollisionClass();
    colclass->set_parent(cache_[parent]);
#ifdef DEBUG
    colclass->set_name(name);
#endif
}
    
void CollisionManager::Update () {
    std::map<std::string, CollisionClass*>::iterator it;
    for (it = cache_.begin(); it != cache_.end(); ++it) {
        (*it).second->Update();
    }
}

} // namespace collision
} // namespace pyramidworks
