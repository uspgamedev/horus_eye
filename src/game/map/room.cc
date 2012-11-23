#include "game/map/room.h"

#include <cfloat>
#include <ugdk/graphic/node.h>

#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/components/graphic.h"

namespace map {

using std::vector;
using std::list;
using sprite::WorldObject;
using scene::BACKGROUND_LAYER;
using scene::FOREGROUND_LAYER;
using ugdk::graphic::Node;

Room::Room(const std::string& name, const ugdk::math::Integer2D& _size, 
    const ugdk::math::Integer2D& _position)
        : name_(name), size_(_size), position_(_position), level_(NULL) {

    floor_ = new Node;
    floor_->set_zindex(-FLT_MAX);
    floor_->modifier()->set_offset(scene::World::FromWorldCoordinates(position_));
}

Room::~Room() {
}

void Room::Update(double dt) {
    updateObjects(dt);
    deleteToBeRemovedObjects();
    flushObjectQueue();
}

void Room::AddObject(sprite::WorldObject* obj) {
    if(!level_)
        handleNewObject(obj);
    else
        queued_objects_.push(obj);
}

void Room::AddObject(sprite::WorldObject* obj, const ugdk::Vector2D& position, bool absolute) {
    if(absolute == POSITION_ABSOLUTE)
        obj->set_world_position(position);
    else
        obj->set_world_position(position + position_);
    AddObject(obj);
}

void Room::DefineLevel(scene::World* level) {
    level_ = level;
}

WorldObject* Room::WorldObjectByTag (const std::string& tag) const {
    TagTable::const_iterator match = tagged_.find(tag);
    if (match == tagged_.end()) return NULL;
    return match->second;
}
    
void Room::RemoveTag(const std::string& tag) {
    tagged_[tag] = NULL;
}

void Room::updateObjects(double delta_t) {
    for(std::list<sprite::WorldObject*>::iterator it = objects_.begin(); it != objects_.end(); ++it)
        (*it)->Update(delta_t);
}

static bool worldobjectIsToBeRemoved (const sprite::WorldObject* value) {
    bool is_dead = value->to_be_removed();
    if (is_dead)
        delete value;
    return is_dead;
}

void Room::deleteToBeRemovedObjects() {
    objects_.remove_if(worldobjectIsToBeRemoved);
}

void Room::flushObjectQueue() {
    while(!queued_objects_.empty()) {
        handleNewObject(queued_objects_.front());
        queued_objects_.pop();
    }
}

void Room::handleNewObject(sprite::WorldObject* obj) {
    objects_.push_back(obj);
    obj->OnRoomAdd(this);
    tagged_[obj->tag()] = obj;
    if(level_ && level_->IsRoomActive(this) && obj->graphic())
        obj->graphic()->InsertIntoLayers(level_->layers());
}
    
} // namespace map
