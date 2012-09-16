#include "game/map/room.h"

#include <ugdk/graphic/node.h>
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"

namespace map {

using std::vector;
using std::list;
using sprite::WorldObject;

Room::Room(const ugdk::math::Integer2D& _size, const GameMap& matrix)
    : floor_(new ugdk::graphic::Node), matrix_(matrix), size_(_size) {}

Room::~Room() {
    delete floor_;
}
   
void Room::AddObject(sprite::WorldObject* obj, const ugdk::Vector2D& position) {
    obj->set_world_position(position);
    objects_.push_back(obj);
    tagged_[obj->tag()] = obj;
}

void Room::AddToWorld(scene::World* world) {
    for(list<WorldObject*>::iterator it = objects_.begin(); it != objects_.end(); ++it) {
        (*it)->set_world_position((*it)->world_position() + offset_);
        world->AddWorldObject(*it);
    }
    floor_->modifier()->set_offset(scene::World::FromWorldCoordinates(offset_));
    world->content_node()->AddChild(floor_);
}

WorldObject* Room::WorldObjectByTag (const std::string& tag) {
    TagTable::iterator match = tagged_.find(tag);
    if (match == tagged_.end()) return NULL;
    return match->second;
}
    
void Room::RemoveTag(const std::string& tag) {
    tagged_[tag] = NULL;
}
    
void Room::AddFloor(ugdk::graphic::Node* floor) {
    floor_->AddChild(floor);
}

} // namespace map
