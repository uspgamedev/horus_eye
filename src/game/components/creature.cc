#include <cmath>
#include <iostream>

#include <ugdk/action/animation.h>
#include <ugdk/action/animationset.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/time/timeaccumulator.h>
#include <ugdk/util/animationparser.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "creature.h"

#include "game/sprites/condition.h"
#include "game/scenes/world.h"
#include "game/skills/skill.h"

using namespace ugdk;
using namespace ugdk::action;
using namespace utils;

using pyramidworks::geometry::Circle;
using sprite::Condition;
using sprite::WorldObject;

#define PI 3.1415926535897932384626433832795

namespace component {

const Direction Creature::direction_mapping_[8] = {
    Direction::Right(),
    Direction::Right() | Direction::Up(),
    Direction::Up(),
    Direction::Up() | Direction::Left(),
    Direction::Left(),
    Direction::Left() | Direction::Down(),
    Direction::Down(),
    Direction::Down() | Direction::Right()
};
uint32 Creature::standing_animations_[16];
uint32 Creature::walking_animations_[16];
uint32 Creature::attacking_animations_[8];
uint32 Creature::taking_damage_animation_ = -1;
uint32 Creature::dying_animation_ = -1;
Vector2D Creature::directions_[4];

AnimationSet* Creature::ANIMATIONS = NULL;

COLLISION_DIRECT(Creature*, RectCollision, obj) {
    WorldObject *wobj = (WorldObject *) obj;
    data_->CollideWithRect(wobj->collision_object());
}

Creature::Creature(WorldObject* owner, Controller* controller)
    :   owner_(owner),
        waiting_animation_(false),
        last_standing_direction_(Direction::Down()),
        animation_direction_(),
        last_stable_position_(),
        last_dt_(0.0),
        sight_count_(0),
        aim_(owner->world_position(), controller->aim_destination()),
        sprite_(NULL) {
            owner_->set_logic(this);
            if(!owner_->controller())
                owner_->set_controller(controller);
            if(!owner_->collision_object() && WORLD())
                owner_->set_collision_object(new pyramidworks::collision::CollisionObject(WORLD()->collision_manager(), owner_));
}

Creature::~Creature() {
    std::map<Controller::SkillSlot, skills::Skill*>::iterator it;
    for(it = active_skills_.begin(); it != active_skills_.end(); ++it)
        delete it->second;
}

void Creature::Initialize(ugdk::graphic::Spritesheet *image, ugdk::action::AnimationSet *set) {
    owner_->node()->set_drawable(sprite_ = new ugdk::graphic::Sprite(image, ANIMATIONS));
    sprite_->AddObserverToAnimation(this);
}

void Creature::AddKnownCollisions() {
    // Teach this creature how to collides with Walls.
    owner_->collision_object()->AddCollisionLogic("Wall", new RectCollision(this));
}

bool deletecondition(Condition *condition) {
    bool is_finished = (condition->phase() == Condition::PHASE_FINISHED);
    if (is_finished) delete condition;
    return is_finished;
}

bool Creature::AddCondition(Condition* new_condition) {
    conditions_.push_front(new_condition);
    new_condition->StartCondition(this);
    return true;
}

void Creature::UpdateCondition(double dt) {
     std::list<Condition*>::iterator i;
     for (i = conditions_.begin(); i != conditions_.end(); ++i) 
         (*i)->Update(dt);
     conditions_.remove_if(deletecondition);
}

// ANIMATION STUFF

void Creature::InitializeAnimations() {
    if (ANIMATIONS != NULL) return;

    ANIMATIONS = ugdk::base::ResourceManager::GetAnimationSetFromFile("animations/creature.gdd");
    InitializeAttackingAnimations();
    InitializeWalkingAnimations();
    InitializeStandingAnimations();
    taking_damage_animation_ = ANIMATIONS->MakeIndex("TAKING_DAMAGE");
    dying_animation_ = ANIMATIONS->MakeIndex("DYING");

    directions_[Direction_::RIGHT] = Vector2D(1, -1);
    directions_[Direction_::LEFT] = Vector2D(-1, 1);
    directions_[Direction_::DOWN] =  Vector2D(-1, -1);
    directions_[Direction_::UP] = Vector2D(1, 1);
}

void Creature::InitializeAttackingAnimations() {
    attacking_animations_[6] = ANIMATIONS->MakeIndex("ATTACKING_DOWN");
    attacking_animations_[4] = ANIMATIONS->MakeIndex("ATTACKING_LEFT");
    attacking_animations_[0] = ANIMATIONS->MakeIndex("ATTACKING_RIGHT");
    attacking_animations_[2] = ANIMATIONS->MakeIndex("ATTACKING_UP");
    attacking_animations_[7] = ANIMATIONS->MakeIndex("ATTACKING_DOWN_RIGHT");
    attacking_animations_[5] = ANIMATIONS->MakeIndex("ATTACKING_DOWN_LEFT");
    attacking_animations_[1] = ANIMATIONS->MakeIndex("ATTACKING_UP_RIGHT");
    attacking_animations_[3] = ANIMATIONS->MakeIndex("ATTACKING_UP_LEFT");
}

void Creature::InitializeWalkingAnimations() {
    for (int i = 0; i < 16; i++) walking_animations_[i] = -1;

    walking_animations_[Animation_::DOWN                    ] = ANIMATIONS->MakeIndex("WALKING_DOWN");
    walking_animations_[Animation_::UP                      ] = ANIMATIONS->MakeIndex("WALKING_UP");
    walking_animations_[                   Animation_::LEFT ] = ANIMATIONS->MakeIndex("WALKING_LEFT");
    walking_animations_[                   Animation_::RIGHT] = ANIMATIONS->MakeIndex("WALKING_RIGHT");
    walking_animations_[Animation_::DOWN | Animation_::RIGHT] = ANIMATIONS->MakeIndex("WALKING_DOWN_RIGHT");
    walking_animations_[Animation_::DOWN | Animation_::LEFT ] = ANIMATIONS->MakeIndex("WALKING_DOWN_LEFT");
    walking_animations_[Animation_::UP   | Animation_::RIGHT] = ANIMATIONS->MakeIndex("WALKING_UP_RIGHT");
    walking_animations_[Animation_::UP   | Animation_::LEFT ] = ANIMATIONS->MakeIndex("WALKING_UP_LEFT");
}

void Creature::InitializeStandingAnimations() {
    for (int i = 0; i < 16; i++) standing_animations_[i] = -1;

    standing_animations_[Animation_::DOWN                    ] = ANIMATIONS->MakeIndex("STANDING_DOWN");
    standing_animations_[                   Animation_::LEFT ] = ANIMATIONS->MakeIndex("STANDING_LEFT");
    standing_animations_[                   Animation_::RIGHT] = ANIMATIONS->MakeIndex("STANDING_RIGHT");
    standing_animations_[Animation_::UP                      ] = ANIMATIONS->MakeIndex("STANDING_UP");
    standing_animations_[Animation_::DOWN | Animation_::RIGHT] = ANIMATIONS->MakeIndex("STANDING_DOWN_RIGHT");
    standing_animations_[Animation_::DOWN | Animation_::LEFT ] = ANIMATIONS->MakeIndex("STANDING_DOWN_LEFT");
    standing_animations_[Animation_::UP   | Animation_::RIGHT] = ANIMATIONS->MakeIndex("STANDING_UP_RIGHT");
    standing_animations_[Animation_::UP   | Animation_::LEFT ] = ANIMATIONS->MakeIndex("STANDING_UP_LEFT");
}


// ============= other stuff

void Creature::Update(double dt) {
    UpdateCondition(dt);
    mana_.Update(dt);
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
    const pyramidworks::geometry::Circle *circle =
        (const pyramidworks::geometry::Circle*) owner_->collision_object()->shape();

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

void Creature::Tick() {
    if (owner_->status() == WorldObject::STATUS_DYING) {
        owner_->Die();
    }
    waiting_animation_ = false;
}

int Creature::GetAttackingAnimationIndex(double angle) {
    int degreeAngle = (int)((angle / PI) * 360);
    degreeAngle += 45;
    int animationIndex = degreeAngle / 90;
    return animationIndex % 8;
}

double Creature::GetAttackingAngle(Vector2D targetDirection) {
    Vector2D versor = Vector2D::Normalized(targetDirection);
    double radianAngle = acos(versor.x);
    if (versor.y > 0) {
        radianAngle = 2*PI - radianAngle;
    }
    return radianAngle;
}

}  // namespace sprite