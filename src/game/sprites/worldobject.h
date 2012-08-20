#ifndef HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_

#include <string>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include <ugdk/math/vector2D.h>
#include <ugdk/action/entity.h>
#include <ugdk/graphic.h>
#include <ugdk/time.h>
#include <pyramidworks/collision.h>
#include <pyramidworks/geometry.h>
#include "game/scenes/gamelayer.h"
#include "game/components.h"

namespace sprite {

class WorldObject : public ugdk::action::Entity {
  public:
    /** @param duration Sets timed life to the given value, if positive. */
    WorldObject(double duration = -1.0);
    ~WorldObject();

    /** An WorldObject may be one of three states:
      * STATUS_ACTIVE: this object is operating normally.
      * STATUS_DYING:  this object is dying and should be ready to die soon.
      * STATUS_DEAD:   this object is dead and will be deleted at the end of the frame. */
    enum Status { STATUS_ACTIVE, STATUS_DYING, STATUS_DEAD };

    // The BIG Awesome update method. TODO explain better
    void Update(double dt);

    void Die();
    void StartToDie();

    void set_identifier(const std::string& identifier) { identifier_ = identifier; }
    const std::string& identifier() const { return identifier_; }

    void set_tag(const std::string& tag) { tag_ = tag; }
    const std::string& tag() const { return tag_; }

    const ugdk::Vector2D& world_position() const { return world_position_; }
    void set_world_position(const ugdk::Vector2D& pos);

    Status status() const { return status_; }
    bool is_active() const { return status_ == STATUS_ACTIVE; }

    // The Light radius. TODO explain better
    double light_radius() const { return light_radius_; }
    void set_light_radius(double radius);

    void set_collision_object(pyramidworks::collision::CollisionObject* col) { collision_object_ = col; }
    pyramidworks::collision::CollisionObject* collision_object() const { return collision_object_; }

    void set_shape(pyramidworks::geometry::GeometricShape* shape);

          ugdk::graphic::Node* node();
    const ugdk::graphic::Node* node() const;

    void set_timed_life(ugdk::time::TimeAccumulator*);
    void set_timed_life(double);
    ugdk::time::TimeAccumulator* timed_life() { return timed_life_; }

    void OnSceneAdd(ugdk::action::Scene* scene);

    void set_start_to_die_callback(std::tr1::function<void (WorldObject*)> on_death_start_callback) {
        on_start_to_die_callback_ = on_death_start_callback;
    }

    void set_die_callback(std::tr1::function<void (WorldObject*)> on_death_end_callback) {
        on_die_callback_ = on_death_end_callback;
    }

    void set_logic(component::Logic* logic) { logic_ = logic; }
    component::Logic* logic() { return logic_; }

    void set_damageable(component::Damageable* damageable) { damageable_ = damageable; }
    component::Damageable* damageable() { return damageable_; }

    void set_graphic(component::Graphic* graphic) { graphic_ = graphic; }
    component::Graphic* graphic() { return graphic_; }

    void set_controller(component::Controller* controller) { controller_ = controller; }
    component::Controller* controller() { return controller_; }

    void set_animation(component::Animation* animation) { animation_ = animation; }
    component::Animation* animation() { return animation_; }

    void set_layer(scene::GameLayer layer) { layer_ = layer; }
    scene::GameLayer layer() const { return layer_; }

  protected:
    std::string identifier_;

    // Collision component
    pyramidworks::collision::CollisionObject *collision_object_;

    // 
    ugdk::time::TimeAccumulator* timed_life_;

    // TODO: make this somethintg
    std::tr1::function<void (WorldObject*)> on_start_to_die_callback_;
    std::tr1::function<void (WorldObject*)> on_die_callback_;

  private:
    // The object's position in World's coordinate system. Should be handled by the set_world_position and world_position methods.
    ugdk::Vector2D world_position_;

    std::string tag_;

    // The current status for the object.
    Status status_;
    double light_radius_;
    scene::GameLayer layer_;

    component::Damageable* damageable_;

    component::Graphic* graphic_;

    component::Logic* logic_;

    component::Controller* controller_;

    component::Animation* animation_;

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
