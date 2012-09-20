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
    content_node()->AddChild(layers_[BACKGROUND_LAYER]);
    content_node()->AddChild(layers_[FOREGROUND_LAYER]);

    layers_[BACKGROUND_LAYER]->modifier()->set_offset(scene::World::FromWorldCoordinates(position_));
}

Room::~Room() {
}
   
void Room::AddObject(sprite::WorldObject* obj, const ugdk::Vector2D& position) {
    obj->set_world_position(position + position_);
    tagged_[obj->tag()] = obj;
    this->QueuedAddEntity(obj);
}

WorldObject* Room::WorldObjectByTag (const std::string& tag) {
    TagTable::iterator match = tagged_.find(tag);
    if (match == tagged_.end()) return NULL;
    return match->second;
}
    
void Room::RemoveTag(const std::string& tag) {
    tagged_[tag] = NULL;
}
    
} // namespace map
