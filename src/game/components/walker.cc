#include "walker.h"

#include <cmath>

#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "game/components/animation.h"
#include "game/components/controller.h"
#include "game/components/caster.h"
#include "game/components/shape.h"
#include "game/sprites/worldobject.h"
#include "game/scenes/world.h"
#include "game/skills/skill.h"
#include "game/utils/geometryprimitives.h"
#include "game/map/tile.h"

using namespace ugdk;
using namespace ugdk::action;
using namespace utils;

using map::Tile;
using map::GameMap;
using map::TilePos;
using sprite::WorldObject;

namespace component {

COLLISION_DIRECT(Walker*, RectCollision, obj) {
    WorldObject *wobj = (WorldObject *) obj;
    data_->collideWithRect(wobj->shape()->collision());
}

pyramidworks::collision::CollisionLogic* CreateWalkerRectCollision(Walker* obj) {
    return new RectCollision(obj);
}

Walker::Walker(WorldObject* owner, double original_speed)
    :   owner_(owner),
        last_dt_(0.0),
        current_speed_(original_speed),
        original_speed_(original_speed) {}

Walker::~Walker() {}

// ============= other stuff

void Walker::Update(double dt) {
    if(!owner_->dead()) {
        component::Controller* controller = owner_->controller();
        component::Animation* animation = owner_->component<Animation>();

        const Direction& direction = controller->direction();
        if(direction && animation && animation->CanInterrupt(utils::MOVEMENT)) {
            animation->ChangeAnimation(utils::MOVEMENT, direction);
        }

        if(!animation || animation->IsAnimation(utils::MOVEMENT)) {
            if(!direction) {
                if(animation) animation->FinishAnimation();
            } else {
                if(animation) animation->ChangeDirection(direction);

                walking_direction_ = (controller->direction_vector() + offset_direction_).Normalize();
                move(walking_direction_, dt);
                
                offset_direction_.x = 0.0;
                offset_direction_.y = 0.0;
            }
        }
    }
    current_speed_ = original_speed_;
}

void Walker::move(Vector2D direction, double delta_t) {
    // If you called Move() you should be in a stable position.

    last_stable_position_ = owner_->world_position();
    last_dt_ = delta_t;

    owner_->set_world_position(last_stable_position_ + direction * (current_speed_ * delta_t));
}

void Walker::collideWithRect(const pyramidworks::collision::CollisionObject* coll_obj) {
    // rollback to the last stable position.
    owner_->set_world_position(last_stable_position_);

    // Get all values we'll need
    //const pyramidworks::geometry::Circle *circle =
    //    (const pyramidworks::geometry::Circle*) owner_->collision_object()->shape();

    const pyramidworks::geometry::Rect *rect = 
        (const pyramidworks::geometry::Rect*) coll_obj->shape();

    Vector2D circ_pos = owner_->shape()->collision()->absolute_position();
    Vector2D rect_pos = coll_obj->absolute_position();

    double half_r_width  = rect->width() /2.0;
    double half_r_height = rect->height()/2.0;

    double r_left   = rect_pos.x - half_r_width;
    double r_right  = rect_pos.x + half_r_width;
    double r_bottom = rect_pos.y - half_r_height;
    double r_top    = rect_pos.y + half_r_height;

    // first, if the collision is trivial (in other words, non-corner):
    if(circ_pos.y <= r_top && circ_pos.y >= r_bottom)
        walking_direction_.x = 0;
    else if(circ_pos.x <= r_right && circ_pos.x >= r_left)
        walking_direction_.y = 0;
    // otherwise, we'll have to do some calculations:
    else {
        // find the intersection point. Yep, it's a point -- the rect corner.
        Vector2D intersection = Vector2D(r_left, r_bottom);
        if (circ_pos.x >= r_right) intersection.x = r_right;
        if (circ_pos.y >= r_top  ) intersection.y = r_top;

        // impede movement in the wall direction.
        Vector2D radius = circ_pos - intersection;
        radius = radius.Normalize();
        walking_direction_ = walking_direction_ + radius;
    }

    // normalize the walking_direction_ and move correctly this time.
    walking_direction_ = walking_direction_.Normalize();
    move(walking_direction_, last_dt_);
}

}  // namespace component
