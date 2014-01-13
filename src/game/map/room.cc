#include "game/map/room.h"

#include <cfloat>
#include <ugdk/graphic/node.h>

#include "game/builders/scriptbuilder.h"
#include "game/core/coordinates.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/components/graphic.h"
#include "game/components/body.h"

#include <ugdk/internal/opengl.h>
#include <ugdk/debug/profiler.h>

namespace map {

using std::vector;
using std::list;
using sprite::WorldObject;
using scene::BACKGROUND_LAYER;
using scene::FOREGROUND_LAYER;
using ugdk::graphic::Node;
using ugdk::script::VirtualObj;

Room::Room(const std::string& name, const ugdk::math::Integer2D& _size, 
    const ugdk::math::Integer2D& _position)
        : name_(name), size_(_size), position_(_position), level_(NULL) {

    floor_ = new Node;
    floor_->set_zindex(-FLT_MAX);
    floor_->geometry().set_offset(core::FromWorldCoordinates(position_));
    layers_[BACKGROUND_LAYER] = new Node;
    layers_[FOREGROUND_LAYER] = new Node;
}

Room::~Room() {
    delete floor_;
    delete layers_[BACKGROUND_LAYER];
    delete layers_[FOREGROUND_LAYER];
}

void Room::Update(double dt) {
    updateObjects(dt);
    deleteToBeRemovedObjects();
    flushObjectQueue();
}
    
void Room::Render(ugdk::graphic::Canvas& canvas) const {
    ugdk::debug::ProfileSection section("Room '" + name_ + "'");

    floor_->Render(canvas);

    glEnable(GL_DEPTH_TEST);

    for(const auto& obj : objects_)
        if(const auto& graphic = obj->graphic())
            graphic->Render(canvas);
    
    glDisable(GL_DEPTH_TEST);
}
    
void Room::RenderLight(ugdk::graphic::Canvas& canvas) const {
    ugdk::debug::ProfileSection section("Room '" + name_ + "'");

    for(const auto& obj : objects_)
        if(const auto& graphic = obj->graphic())
            graphic->RenderLight(canvas);
}

void Room::AddObject(sprite::WorldObject* obj) {
    if(!level_)
        handleNewObject(obj);
    else
        queued_objects_.push(obj);
}

void Room::AddObject(sprite::WorldObject* obj, const ugdk::math::Vector2D& position, bool absolute) {
    if(absolute == POSITION_ABSOLUTE)
        obj->set_world_position(position);
    else
        obj->set_world_position(position + position_);
    AddObject(obj);
}

void Room::ForceAddObject(sprite::WorldObject* obj) {
    handleNewObject(obj);
}

void Room::RemoveObject(sprite::WorldObject* obj) {
    objects_.remove(obj);
    if(!obj->tag().empty())
        RemoveTag(obj->tag());
}

void Room::MakeRecipe(const std::string& recipe_name, const ugdk::math::Vector2D& position, const std::string& tag, bool absolute) {
    VirtualObj recipe = recipes_[recipe_name];
    if(!recipe) {
        fprintf(stderr, "Warning: Could not find recipe '%s' in room '%s'.\n", recipe_name.c_str(), name_.c_str());
        return;
    }
    WorldObject* wobj = builder::ScriptBuilder::Script(recipe["property"].value<std::string>(), recipe["params"]);
    if(wobj) {
        wobj->set_tag(tag);
        AddObject(wobj, position, absolute);
    }
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
    
void Room::Activate() {
    assert(level_);
    for(WorldObject* wobj : objects_)
        if(auto b = wobj->body())
            b->Activate(level_);
}

void Room::Deactivate() {
    for(WorldObject* wobj : objects_)
        if(auto b = wobj->body())
            b->Deactivate();
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
    if(!obj->tag().empty())
        tagged_[obj->tag()] = obj;
    if(level_ && level_->IsRoomActive(this))
        if(auto b = obj->body())
            b->Activate(level_);
}
    
} // namespace map
