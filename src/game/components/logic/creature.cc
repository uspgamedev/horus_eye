#include <cmath>
#include <iostream>

#include <ugdk/action/animation.h>
#include <ugdk/action/animationset.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "creature.h"

#include "game/components/animation.h"
#include "game/components/controller.h"
#include "game/components/caster.h"
#include "game/sprites/condition.h"
#include "game/sprites/worldobject.h"
#include "game/scenes/world.h"
#include "game/skills/skill.h"
#include "game/utils/geometryprimitives.h"
#include "game/utils/tile.h"

using namespace ugdk;
using namespace ugdk::action;
using namespace utils;

using pyramidworks::geometry::Circle;
using sprite::Condition;
using sprite::WorldObject;

#define PI 3.1415926535897932384626433832795

namespace component {

COLLISION_DIRECT(Creature*, RectCollision, obj) {
    WorldObject *wobj = (WorldObject *) obj;
    data_->CollideWithRect(wobj->collision_object());
}

Creature::Creature(WorldObject* owner, double speed)
    :   owner_(owner),
        last_standing_direction_(Direction::Down()),
        last_dt_(0.0),
        speed_(speed),
        original_speed_(speed) {}

Creature::~Creature() {}

void Creature::AddKnownCollisions() {
    // Teach this creature how to collides with Walls.
    owner_->collision_object()->AddCollisionLogic("Wall", new RectCollision(this));
}

// ============= other stuff

void Creature::Update(double dt) {
    scene::World *world = WORLD();
    if (world) {
        GameMap& map = world->level_matrix();
        TilePos mummy_pos = Tile::ToTilePos(owner_->world_position());
        mummy_pos.i =  map.size() - mummy_pos.i - 1;
        Tile *mummy_tile = Tile::GetFromMapPosition(map, mummy_pos);
        if (mummy_tile) {
            if(mummy_tile->visible())
                owner_->node()->modifier()->set_visible(true);
            else
                owner_->node()->modifier()->set_visible(false);
        }
    }
    if(owner_->is_active()) {
        component::Controller* controller = owner_->controller();
        if(!owner_->animation()->is_uninterrutible()) {
            UseSkills();
        }
        if(!owner_->animation()->is_uninterrutible()) {
            walking_direction_ = controller->direction_vector();
            const Direction& direction = controller->direction();
            if(direction) {
                last_standing_direction_ = direction;
                owner_->animation()->set_animation(utils::WALKING);
                owner_->animation()->set_direction(direction);
            } else {
                owner_->animation()->set_animation(utils::STANDING);
                owner_->animation()->set_direction(last_standing_direction_);
            }
            Creature::Move(walking_direction(), dt);
        }
    }
    speed_ = original_speed_;
}

void Creature::UseSkills() {
    Caster* caster = owner()->caster();
    Controller* controller = owner_->controller();
    for(Controller::SkillSlot slot = Controller::PRIMARY; slot < Controller::INVALID_SLOT; slot = Controller::SkillSlot(slot + 1)) {
        skills::Skill* skill = caster->SkillAt(slot);
        if(!skill) continue;
        if(controller->IsUsingSkillSlot(slot) && skill->Available()) {
            if(skill->IsValidUse()) {
                skill->Use();
                StartAttackAnimation();
                break;
            }
        }
    }
}

void Creature::StartAttackAnimation() {
    const skills::usearguments::Aim& aim = owner_->caster()->aim();
    Direction d = Direction::FromWorldVector(aim.destination_ - aim.origin_);
    last_standing_direction_ = d;
    owner_->animation()->set_direction(d);
    owner_->animation()->set_animation(utils::ATTACKING);
    owner_->animation()->flag_uninterrutible();
}

void Creature::Move(Vector2D direction, double delta_t) {
    // If you called Move() you should be in a stable position.
    Vector2D position(owner_->world_position().x, owner_->world_position().y);
    last_stable_position_ = position;
    // Now update the position.
    last_dt_ = delta_t;
    position = position + direction * (this->speed_ * delta_t);
    owner_->set_world_position(position);
}

void Creature::CollideWithRect(const pyramidworks::collision::CollisionObject* coll_obj) {
    // rollback to the last stable position.
    owner_->set_world_position(last_stable_position_);

    // Get all values we'll need
    //const pyramidworks::geometry::Circle *circle =
    //    (const pyramidworks::geometry::Circle*) owner_->collision_object()->shape();

    const pyramidworks::geometry::Rect *rect = 
        (const pyramidworks::geometry::Rect*) coll_obj->shape();

    Vector2D circ_pos = owner_->collision_object()->absolute_position();
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
    Move(walking_direction_, last_dt_);
}

}  // namespace sprite
