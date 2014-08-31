#include "game/map/room.h"

#include <cfloat>
#include <ugdk/graphic/node.h>

#include "game/builders/scriptbuilder.h"
#include "game/core/coordinates.h"
#include "game/scenes/lightrendering.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/sprites/objecthandle.h"
#include "game/components/graphic.h"
#include "game/components/lightemitter.h"
#include "game/components/body.h"
#include "game/initializer.h"

#include <ugdk/internal/opengl.h>
#include <ugdk/debug/profiler.h>

#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <memory>

namespace map {

using std::vector;
using std::list;
using sprite::WObjPtr;
using ugdk::graphic::Node;
using ugdk::script::VirtualObj;

Room::Room(const std::string& name,
           const ugdk::math::Integer2D& _size,
           const ugdk::math::Integer2D& _position)
    : name_(name)
    , size_(_size)
    , position_(_position)
    , level_(nullptr)
{
    floor_ = new Node;
    floor_->set_zindex(-FLT_MAX);
    floor_->geometry().set_offset(core::FromWorldCoordinates(position_));
}

Room::~Room() {
    delete floor_;
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

    using namespace ugdk::graphic;
        
    const Geometry& geo = canvas.current_geometry();

    auto light_rendering = this->level()->light_rendering();

    const opengl::ShaderProgram* current_shader = nullptr;
    const ugdk::internal::GLTexture* current_texture = nullptr;
    std::unique_ptr<opengl::ShaderUse> shader_use;

    int shader_changes = 0;
    int texture_changes = 0;
    for (const auto& obj : objects_) {
        if (const auto& graphic = obj->graphic()) {
            const auto& primitive = graphic->primitive();

            if (primitive.shader_program() != current_shader) {
                shader_use = nullptr;
                shader_use.reset(new opengl::ShaderUse(current_shader = primitive.shader_program()));
                shader_use->SendGeometry(geo);
                shader_use->SendTexture(1, light_rendering->light_texture(), "light_texture");
                shader_use->SendUniform("LEVEL_SIZE", this->level()->size().x, this->level()->size().y);
                shader_changes++;
            }

            if (primitive.texture() != current_texture) {
                shader_use->SendTexture(0, current_texture = primitive.texture());
                texture_changes++;
            }

            glm::vec4 position_ogl = geo.AsMat4() * glm::vec4(graphic->final_position().x, graphic->final_position().y, 0.0, 0.0);
            glm::vec4 render_off_ogl = geo.AsMat4() * glm::vec4(graphic->render_offset().x, graphic->render_offset().y, 0.0, 0.0);
            Vector2D lightpos = (Vector2D(position_ogl.x, position_ogl.y) + geo.offset() - Vector2D(render_off_ogl.x, render_off_ogl.y))* 0.5 + Vector2D(0.5, 0.5);
            shader_use->SendUniform("objectDepth", lightpos.y);
            shader_use->SendUniform("lightUV", obj->world_position().x / level()->size().x, obj->world_position().y / level()->size().y);

            shader_use->SendEffect(canvas.current_visualeffect() * primitive.visual_effect());
            primitive.drawfunction()(primitive, *shader_use);
        }
    }
    //printf("Room '%s' rendered with %d shader changes and %d texture changes.\n", name_.c_str(), shader_changes, texture_changes);
    
    glDisable(GL_DEPTH_TEST);
}
    
void Room::RenderLight(ugdk::graphic::Canvas& canvas) const {
    ugdk::debug::ProfileSection section("Room '" + name_ + "'");

    for(const auto& obj : objects_)
        if(const auto& graphic = obj->light())
            graphic->Render(canvas);
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

void Room::DefineLevel(scene::World* level) {
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
        if (ag && bg)
            return ag->primitive().texture() < bg->primitive().texture();
        if (ag || bg)
            return ag < bg;
        return a.get() < b.get();
    });
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
