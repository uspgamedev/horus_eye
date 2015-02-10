#include "game/map/room.h"

#include "game/builders/scriptbuilder.h"
#include "game/core/coordinates.h"
#include "game/map/giantfloor.h"
#include "game/core/world.h"
#include "game/sprites/worldobject.h"
#include "game/sprites/objecthandle.h"
#include "game/components/graphic.h"
#include "game/components/body.h"
#include "game/initializer.h"

#include <algorithm>
#include <memory>

namespace map {

using std::vector;
using std::list;
using sprite::WObjPtr;
using ugdk::script::VirtualObj;

Room::Room(const std::string& name,
           const ugdk::math::Integer2D& _size,
           const ugdk::math::Integer2D& _position)
    : name_(name)
    , size_(_size)
    , position_(_position)
    , level_(nullptr)
{
}

Room::~Room() {
}

void Room::Update(double dt) {
    updateObjects(dt);
    deleteToBeRemovedObjects();
    flushObjectQueue();
}
    
void Room::AddObject(const sprite::WObjPtr& obj) {
    if(!level_)
        handleNewObject(obj);
    else
        queued_objects_.push(obj);
}

void Room::AddObject(const sprite::WObjPtr& obj, const ugdk::math::Vector2D& position, bool absolute) {
    if(absolute == POSITION_ABSOLUTE)
        obj->set_world_position(position);
    else
        obj->set_world_position(position + position_);
    AddObject(obj);
}

void Room::ForceAddObject(const sprite::WObjPtr& obj) {
    handleNewObject(obj);
}

void Room::RemoveObject(const sprite::WObjPtr& obj) {
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
    WObjPtr wobj = WObjPtr(builder::ScriptBuilder::Script(recipe["property"].value<std::string>(), recipe["params"]));
    if(wobj) {
        wobj->set_tag(tag);
        AddObject(wobj, position, absolute);
    }
}

void Room::DefineLevel(core::World* level) {
    level_ = level;
}

sprite::ObjectHandle Room::WorldObjectByTag(const std::string& tag) const {
    TagTable::const_iterator match = tagged_.find(tag);
    if (match == tagged_.end()) return sprite::ObjectHandle();
    return match->second;
}
    
void Room::RemoveTag(const std::string& tag) {
    tagged_.erase(tag);
}
    
void Room::Activate() {
    assert(level_);
    for(const auto& wobj : objects_)
        if(auto b = wobj->body())
            b->Activate(level_);
}

void Room::Deactivate() {
    for(const auto& wobj : objects_)
        if(auto b = wobj->body())
            b->Deactivate();
}

void Room::Sort() {
    objects_.sort([](const WObjPtr& a, const WObjPtr& b) {
        auto ag = a->graphic(), bg = b->graphic();
        //if (ag && bg)
        //    return ag->primitive().texture() < bg->primitive().texture();
        if (ag || bg)
            return ag < bg;
        return a.get() < b.get();
    });
}

void Room::set_floor(std::unique_ptr<GiantFloor>&& floor) {
    floor_ = std::move(floor);
}

void Room::updateObjects(double delta_t) {
    for(const auto& wobj : objects_)
        wobj->Update(delta_t);
}

void Room::deleteToBeRemovedObjects() {
    objects_.remove_if([](const WObjPtr& x) -> bool {
        if (x->to_be_removed() && x.use_count() > 1)
            printf("!!Danger!! To be removed WObj %p with refcount (%d). Identifier: %s\n", x.get(), x.use_count(), x->identifier().c_str());
        return x->to_be_removed();
    });
}

void Room::flushObjectQueue() {
    while(!queued_objects_.empty()) {
        handleNewObject(queued_objects_.front());
        queued_objects_.pop();
    }
}

void Room::handleNewObject(const sprite::WObjPtr& obj) {
    objects_.push_back(obj);
    obj->OnRoomAdd(this);
    if(!obj->tag().empty())
        tagged_[obj->tag()] = obj;
    if(level_ && level_->IsRoomActive(this))
        if(auto b = obj->body())
            b->Activate(level_);
}
    
} // namespace map
