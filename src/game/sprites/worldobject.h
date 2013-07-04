#ifndef HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_

#include <string>
#include <list>
#include <functional>
#include <unordered_map>

#include <ugdk/math/vector2D.h>
#include <ugdk/action/entity.h>
#include <ugdk/graphic.h>
#include <ugdk/time.h>
#include <pyramidworks/collision.h>
#include <pyramidworks/geometry.h>

#include "game/scenes.h"
#include "game/components.h"
#include "game/sprites.h"
#include "game/map.h"

#include "game/scenes/gamelayer.h"

namespace sprite {

class WorldObject : public ::ugdk::action::Entity {
  public:

    typedef std::function<void (WorldObject*)> WorldObjectEvent;

    /** @param duration Sets timed life to the given value, if positive. */
    WorldObject(double duration = -1.0);
    ~WorldObject();

    // The BIG Awesome update method. TODO explain better
    void Update(double dt);

    void Die();
    void StartToDie();

    void set_identifier(const std::string& identifier) { identifier_ = identifier; }
    const std::string& identifier() const { return identifier_; }

    void set_tag(const std::string& tag) { tag_ = tag; }
    const std::string& tag() const { return tag_; }

    const ugdk::math::Vector2D& world_position() const { return world_position_; }
    void set_world_position(const ugdk::math::Vector2D& pos);

    bool dead() const { return dead_; }

    void set_timed_life(double);

    void OnRoomAdd(map::Room*);
    
    void set_room_add_callback(std::function<void (WorldObject*, map::Room*)> on_room_add_callback) {
        on_room_add_callback_ = on_room_add_callback;
    }

    void set_start_to_die_callback(std::function<void (WorldObject*)> on_death_start_callback) {
        on_start_to_die_callback_ = on_death_start_callback;
    }

    void AddDeathEvent(const WorldObjectEvent& on_death_end_callback) {
        on_die_callbacks_.push_back(on_death_end_callback);
    }

    component::Damageable* damageable();
    component::BaseGraphic* graphic();
    component::Controller* controller();
    component::Caster* caster();
    component::Shape* shape();

    /// Gets a component with the requested type with the given name.
    /**
    @param T The type of the requested component.
    @param name The name of the requested component.
    @return A pointer to the component of requested type, NULL if not found.
    */
    template<class T>
    T* component(const std::string& name) {
        ComponentsByName::const_iterator it = components_.find(name);
        return (it != components_.end()) ? dynamic_cast<T*>(it->second->component) : NULL;
    }
    
    /// Const version of the component getter.
    /** @see component */
    template<class T>
    const T* component(const std::string& name) const {
        ComponentsByName::const_iterator it = components_.find(name);
        return (it != components_.end()) ? dynamic_cast<const T*>(it->second->component) : NULL;
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
    
    /// Adds a component with the given name an order.
    /** 
    @param component The component to add.
    @param name The name to store it with.
    @param order The order where to place it with.
    */
    void AddComponent(component::Base* component, const std::string& name, int order);

    /** Convinent version of AddComponent that auto-completes the name and order from the default values of the component's type. 
    @param T The type from where to extract the default name and order.
    @param component The component to add.
    */
    template<class T>
    void AddComponent(T* component) { 
        AddComponent(component, T::DEFAULT_NAME(), T::DEFAULT_ORDER());
    }
    
    /** Convinent version of AddComponent that auto-completes just the order from the default values of the component's type.
    @param T The type from where to extract the default order.
    @param component The component to add.
    @param name The name to store it with.
    */
    template<class T>
    void AddComponent(T* component, const std::string& name) { 
        AddComponent(component, name, T::DEFAULT_ORDER());
    }

    /// Removes the component with the given name.
    /** Does not delete the component. Does nothing if there's no component with such name.
    @param name The name of the component to remove.
    */
    void RemoveComponent(const std::string& name);

    map::Room* current_room() const { return current_room_; }

  private:

    // TODO: make this somethintg
    std::function<void (WorldObject*, map::Room*)>  on_room_add_callback_;
    WorldObjectEvent                                on_start_to_die_callback_;
    std::list<WorldObjectEvent>                     on_die_callbacks_;

    // The object's position in World's coordinate system. Should be handled by the set_world_position and world_position methods.
    ugdk::math::Vector2D world_position_;

    /// A tag used for searching for the object.
    std::string tag_;

    /// Internal identifier. Debugging purposes.
    std::string identifier_;

    /// The room this object is currently in. (In practice, the room it was created in.)
    map::Room* current_room_;

    // 
    ugdk::time::TimeAccumulator* timed_life_;

    /// Is this object dead?
    bool dead_;

    struct OrderedComponent {
        component::Base* component;
        int order;

        OrderedComponent(component::Base*, int);
        bool operator == (const component::Base*) const;
    };
    typedef std::list<OrderedComponent> ComponentsByOrder;
    typedef std::unordered_map<std::string, ComponentsByOrder::iterator> ComponentsByName;

    ComponentsByName components_;
    ComponentsByOrder components_order_;

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
