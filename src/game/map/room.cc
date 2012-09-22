#include "game/map/room.h"

#include <ugdk/graphic/node.h>
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"

namespace map {

using std::vector;
using std::list;
using sprite::WorldObject;
using scene::BACKGROUND_LAYER;
using scene::FOREGROUND_LAYER;
using ugdk::graphic::Node;

Room::Room(const std::string& name, const ugdk::math::Integer2D& _size, 
    const ugdk::math::Integer2D& _position, const GameMap& matrix)
        : name_(name), matrix_(matrix), size_(_size), position_(_position) {

    layers_[BACKGROUND_LAYER] = new Node;
    layers_[FOREGROUND_LAYER] = new Node;
    layers_[BACKGROUND_LAYER]->set_zindex(BACKGROUND_LAYER);
    layers_[FOREGROUND_LAYER]->set_zindex(FOREGROUND_LAYER);
    layers_[BACKGROUND_LAYER]->modifier()->set_offset(scene::World::FromWorldCoordinates(position_));
}

Room::~Room() {
}

void Room::AddObject(sprite::WorldObject* obj, const ugdk::Vector2D& position) {
    obj->set_world_position(position);
    tagged_[obj->tag()] = obj;
    queued_objects_.push(obj);
}

void Room::Update(double dt) {
    UpdateObjects(dt);
    DeleteToBeRemovedObjects();
    FlushObjectQueue();
}

WorldObject* Room::WorldObjectByTag (const std::string& tag) {
    TagTable::iterator match = tagged_.find(tag);
    if (match == tagged_.end()) return NULL;
    return match->second;
}
    
void Room::RemoveTag(const std::string& tag) {
    tagged_[tag] = NULL;
}

void Room::UpdateObjects(double delta_t) {
    for(std::list<sprite::WorldObject*>::iterator it = objects_.begin(); it != objects_.end(); ++it)
        (*it)->Update(delta_t);
}

static bool worldobjectIsToBeRemoved (const sprite::WorldObject* value) {
    bool is_dead = value->to_be_removed();
    if (is_dead)
        delete value;
    return is_dead;
}

void Room::DeleteToBeRemovedObjects() {
    objects_.remove_if(worldobjectIsToBeRemoved);
}

void Room::FlushObjectQueue() {
    while(!queued_objects_.empty()) {
        sprite::WorldObject* obj = queued_objects_.front();
        objects_.push_back(obj);
        obj->OnRoomAdd(this);
        queued_objects_.pop();
    }
}
    
} // namespace map
