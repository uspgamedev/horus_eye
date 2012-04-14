#ifndef HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_

#include <string>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/entity.h>
#include <ugdk/graphic.h>
#include <ugdk/time.h>
#include <pyramidworks/collision.h>
#include <pyramidworks/geometry.h>
#include "game/components.h"

namespace sprite {

#define INITIALIZE_COLLISION { if(collision_object_ == NULL) collision_object_ = new pyramidworks::collision::CollisionObject(WORLD()->collision_manager(), this); }

#define SET_COLLISIONCLASS(CLASS)        { collision_object_->InitializeCollisionClass(#CLASS); }
#define SET_COLLISIONSHAPE(SHAPE)        set_shape(SHAPE);
#define ADD_COLLISIONLOGIC(CLASS, LOGIC) { collision_object_->AddCollisionLogic(#CLASS, LOGIC); }

class DeathOp {
  public:
    ~DeathOp() {}
    virtual void Callback() = 0;
protected:
    DeathOp() {}
};

class WorldObject : public ugdk::Entity {
  public:
    /** @param duration Sets timed life to the given value, if positive. */
    WorldObject(double duration = -1.0);
    virtual ~WorldObject();

    /** An WorldObject may be one of three states:
      * STATUS_ACTIVE: this object is operating normally.
      * STATUS_DYING:  this object is dying and should be ready to die soon.
      * STATUS_DEAD:   this object is dead and will be deleted at the end of the frame. */
    enum Status { STATUS_ACTIVE, STATUS_DYING, STATUS_DEAD };

    // The BIG Awesome update method. TODO explain better
    virtual void Update(double dt);

    virtual void Dying(double dt) { Die(); }

    virtual void Die() { status_ = STATUS_DEAD; }
    virtual void StartToDie();

    const std::string& identifier() const { return identifier_; }

    const ugdk::Vector2D& world_position() const { return world_position_; }
    virtual void set_world_position(const ugdk::Vector2D& pos);

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

    void set_death_start_callback(DeathOp* on_death_start_callback) {
        on_death_start_callback_ = on_death_start_callback;
    }

    void set_logic(component::Logic* logic) { logic_ = logic; }
    component::Logic* logic() { return logic_; }

    void set_damageable(component::Damageable* damageable) { damageable_ = damageable; }
    component::Damageable* damageable() { return damageable_; }

    void set_graphic(component::Graphic* graphic) { graphic_ = graphic; }
    component::Graphic* graphic() { return graphic_; }

    void set_controller(component::Controller* controller) { controller_ = controller; }
    component::Controller* controller() { return controller_; }

  protected:
    std::string identifier_;

    // Collision component
    pyramidworks::collision::CollisionObject *collision_object_;

    // 
    ugdk::time::TimeAccumulator* timed_life_;

    // TODO: make this somethintg
    DeathOp* on_death_start_callback_;

  private:
    // The object's position in World's coordinate system. Should be handled by the set_world_position and world_position methods.
    ugdk::Vector2D world_position_;

    // The current status for the object.
    Status status_;
    double light_radius_;

    component::Damageable* damageable_;

    component::Graphic* graphic_;

    component::Logic* logic_;

    component::Controller* controller_;

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
