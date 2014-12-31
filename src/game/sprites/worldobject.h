#ifndef HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/graphic.h>
#include <ugdk/time.h>
#include <pyramidworks/collision.h>
#include <pyramidworks/geometry.h>
#include <pyramidworks/collision/collisiondata.h>

#include "game/scenes.h"
#include "game/components.h"
#include "game/sprites.h"
#include "game/map.h"

#include "game/scenes/gamelayer.h"

#include <string>
#include <list>
#include <functional>
#include <unordered_map>
#include <memory>
#include <forward_list>

namespace sprite {

class WorldObject : public ::pyramidworks::collision::CollisionData, public std::enable_shared_from_this<WorldObject> {
  public:
    static WObjPtr Create();
    ~WorldObject();

    // The BIG Awesome update method. TODO explain better
    void Update(double dt);

    void Remove();

    /// Identifier is the debuging name for the object.
    const std::string& identifier() const { return identifier_; }
    void set_identifier(const std::string& identifier) { identifier_ = identifier; }

    /// 
    const std::string& tag() const { return tag_; }
    void set_tag(const std::string& tag) { tag_ = tag; }

    const ugdk::math::Vector2D& world_position() const { return world_position_; }
    void set_world_position(const ugdk::math::Vector2D& pos);

    bool to_be_removed() const { return to_be_removed_; }

    void OnRoomAdd(map::Room*);
    
    void set_room_add_callback(std::function<void(WObjRawPtr, map::Room*)> on_room_add_callback) {
        on_room_add_callback_ = on_room_add_callback;
    }

    void AddOnRemoveCallback(std::function<void(WObjRawPtr)> on_remove_callback) {
        on_remove_callbacks_.push_back(on_remove_callback);
    }

    component::Damageable* damageable();
    component::Graphic* graphic();
    component::Animator* animator();
    component::Animation* animation();
    component::Controller* controller();
    component::Caster* caster();
    component::Body* body();
    component::LightEmitter* light();

    /// Gets a component with the requested type with the given name.
    /**
    @param T The type of the requested component.
    @param name The name of the requested component.
    @return A pointer to the component of requested type, NULL if not found.
    */
    template<class T>
    T* component(const std::string& name) {
        ComponentsByName::const_iterator it = components_.find(name);
        return (it != components_.end()) ? dynamic_cast<T*>(it->second->get()) : nullptr;
    }
    
    /// Const version of the component getter.
    /** @see component */
    template<class T>
    const T* component(const std::string& name) const {
        ComponentsByName::const_iterator it = components_.find(name);
        return (it != components_.end()) ? dynamic_cast<const T*>(it->second->get()) : nullptr;
    }

    /** Convinent version where the component name comes from the default value of the given type. 
    @see component */
    template<class T>
    T* component() { return component<T>(T::DEFAULT_NAME()); }
    
    /** Convinent version where the component name comes from the default value of the given type. 
    @see component */
    template<class T>
    const T* component() const { return component<T>(T::DEFAULT_NAME()); }

    /// Checks if the object has a component of the given name.
    /** @return True if a component is found, false otherwise. */
    bool HasComponent(const std::string& name) const {
        return components_.find(name) != components_.end();
    }
    
    /// Adds a component to this object.
    /** 
    @param component The component to add.
    */
    void AddComponent(component::Base* component);

    /// Removes the component with the given name.
    /** Does not delete the component. Does nothing if there's no component with such name.
    @param name The name of the component to remove.
    */
    void RemoveComponent(const std::string& name);

    map::Room* current_room() const { return current_room_; }

  private:
    WorldObject();
    
    /// Internal identifier. Debugging purposes.
    std::string identifier_;

    /// A tag used for searching for the object.
    std::string tag_;

    // TODO: make this somethintg
    std::function<void(WObjRawPtr, map::Room*)>  on_room_add_callback_;

    /// Functions that are called when this object is removed.
    std::list< std::function<void(WObjRawPtr)> >  on_remove_callbacks_;

    // The object's position in World's coordinate system. Should be handled by the set_world_position and world_position methods.
    ugdk::math::Vector2D world_position_;

    /// The room this object is currently in. (In practice, the room it was created in.)
    map::Room* current_room_;

    /// Should this object memory be freed when the frame ends?
    bool to_be_removed_;

    typedef std::forward_list<std::unique_ptr<component::Base>> ComponentsByOrder;
    typedef std::unordered_map<std::string, ComponentsByOrder::iterator> ComponentsByName;

    ComponentsByName components_;
    ComponentsByOrder components_order_;

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
