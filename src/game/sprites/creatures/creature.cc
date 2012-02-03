#include <cmath>
#include <iostream>

#include <ugdk/action/animation.h>
#include <ugdk/action/animationset.h>
#include <ugdk/util/animationparser.h>
#include <ugdk/base/engine.h>

#include "creature.h"

#include <pyramidworks/geometry/circle.h>

using namespace ugdk;
using namespace utils;

using pyramidworks::geometry::Circle;

#define PI 3.1415926535897932384626433832795

namespace sprite {

int Creature::direction_mapping_[8];
uint32 Creature::standing_animations_[16];
uint32 Creature::walking_animations_[16];
uint32 Creature::attacking_animations_[8];
uint32 Creature::taking_damage_animation_ = -1;
uint32 Creature::dying_animation_ = -1;
Vector2D Creature::directions_[4];

AnimationSet* Creature::ANIMATIONS = NULL;

COLLISION_DIRECT(Creature*, RectCollision, obj) {
    WorldObject *wobj = (WorldObject *)obj;
    const pyramidworks::geometry::Rect *rect = 
        (const pyramidworks::geometry::Rect*) wobj->collision_object()->shape();
    data_->CollideWithRect(rect);
}

Creature::Creature()
    :   WorldObject(),
        waiting_animation_(false),
        animation_direction_(0),
        weapon_(NULL),
        last_stable_position_(),
        last_dt_(0.0f),
        sight_count_(0),
        super_armor_(false),
        invulnerability_time_(0),
        blink_time_(new TimeAccumulator(75)),
        hit_duration_(new TimeAccumulator(0)),
        aim_(world_position_, aim_destination_),
        sprite_(NULL),
        blink_(false) {

    INITIALIZE_COLLISION;
    // Teach this creature how to collides with Walls.
    ADD_COLLISIONLOGIC(Wall, new RectCollision(this));
}

Creature::Creature(resource::Energy &life, resource::Energy &mana)
    :   WorldObject(),
        waiting_animation_(false),
        animation_direction_(0),
        weapon_(NULL),
        last_stable_position_(),
        life_(life),
        mana_(mana),
        sight_count_(0),
        super_armor_(false),
        invulnerability_time_(0),
        blink_time_(new TimeAccumulator(75)),
        hit_duration_(new TimeAccumulator(0)),
        aim_(world_position_, aim_destination_),
        sprite_(NULL),
        blink_(false) {

    INITIALIZE_COLLISION;
    // Teach this creature how to collides with Walls.
    ADD_COLLISIONLOGIC(Wall, new RectCollision(this));
}

Creature::~Creature() {
    if (hit_duration_) delete hit_duration_;
    if (blink_time_) delete blink_time_;
}

void Creature::Initialize(ugdk::graphic::Spritesheet *image, ugdk::AnimationSet *set) {
    this->node()->set_drawable(sprite_ = new ugdk::graphic::Sprite(image, ANIMATIONS));
    sprite_->AddObserverToAnimation(this);
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

void Creature::UpdateCondition(float dt) {
	 std::list<Condition*>::iterator i;
	 for (i = conditions_.begin(); i != conditions_.end(); ++i) 
		 (*i)->Update(dt);
	 conditions_.remove_if(deletecondition);
}

void Creature::AdjustBlink(float delta_t) {
    if (!hit_duration_->Expired()) {
        if (blink_time_->Expired()) {
            blink_ = !blink_;
            node()->modifier()->set_alpha(blink_ ? 1.0f : 0.20f);
            blink_time_->Restart();
        }
    } else 
        node()->modifier()->set_alpha(1.0f);
}

void Creature::TakeDamage(float life_points) {
    if(!hit_duration_->Expired()) return;
#ifdef DEBUG
    int creature_id = static_cast<int>(reinterpret_cast<uintptr_t>(this) & 0xFFFFFF);
    fprintf(stderr, "Damage to %s [%X]. DMG: %.2f; Life: %.2f -> %.2f\n", identifier_.c_str(), creature_id,
        life_points, (float) life_, (float) life_ - life_points);
#endif
    PlayHitSound();
    life_ -= life_points;
    if(life_.Empty()) {
        if (status_ == WorldObject::STATUS_ACTIVE) {
            sprite_->SelectAnimation(dying_animation_);
            this->status_ = WorldObject::STATUS_DYING;
	        StartToDie();
#ifdef DEBUG
            fprintf(stderr, "\tTriggering death animation.\n");
#endif
        }
    } else if(!super_armor_) {
        waiting_animation_ = true;
        sprite_->SelectAnimation(taking_damage_animation_);
    }
    hit_duration_->Restart(invulnerability_time_);
    blink_time_->Restart();
}

// ANIMATION STUFF

void Creature::InitializeAnimations() {
    if (ANIMATIONS != NULL) return;

    ANIMATIONS = Engine::reference()->animation_loader().Load("animations/creature.gdd");
    InitializeAttackingAnimations();
    InitializeWalkingAnimations();
    InitializeStandingAnimations();
    taking_damage_animation_ = ANIMATIONS->MakeIndex("TAKING_DAMAGE");
    dying_animation_ = ANIMATIONS->MakeIndex("DYING");

    directions_[Direction_::RIGHT] = Vector2D(1, -1);
    directions_[Direction_::LEFT] = Vector2D(-1, 1);
    directions_[Direction_::DOWN] =  Vector2D(-1, -1);
    directions_[Direction_::UP] = Vector2D(1, 1);

    direction_mapping_[0] = Animation_::RIGHT;
    direction_mapping_[1] = Animation_::RIGHT | Animation_::UP;
    direction_mapping_[2] = Animation_::UP;
    direction_mapping_[3] = Animation_::UP | Animation_::LEFT;
    direction_mapping_[4] = Animation_::LEFT;
    direction_mapping_[5] = Animation_::LEFT | Animation_::DOWN;
    direction_mapping_[6] = Animation_::DOWN;
    direction_mapping_[7] = Animation_::DOWN | Animation_::RIGHT;
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

void Creature::Update(float dt) {
    WorldObject::Update(dt);
    UpdateCondition(dt);
    life_.Update(dt);
    mana_.Update(dt);
}

void Creature::Move(Vector2D direction, float delta_t) {
    // If you called Move() you should be in a stable position.
    Vector2D position(this->world_position().x, this->world_position().y);
    last_stable_position_ = position;
    // Now update the position.
    last_dt_ = delta_t;
    position = position + direction * (this->speed_ * delta_t);
    set_world_position(position);
}

void Creature::CollideWithRect(const pyramidworks::geometry::Rect *rect) {
    // rollback to the last stable position.
    set_world_position(last_stable_position_);

    // Get all values we'll need
    const pyramidworks::geometry::Circle *circle = 
        (const pyramidworks::geometry::Circle*) collision_object_->shape();

    Vector2D circ_pos = circle->position();
    Vector2D rect_pos = rect->position();

    float half_r_width  = rect->width() /2.0f;
    float half_r_height = rect->height()/2.0f;

    float r_left   = rect_pos.x - half_r_width;
    float r_right  = rect_pos.x + half_r_width;
    float r_bottom = rect_pos.y - half_r_height;
    float r_top    = rect_pos.y + half_r_height;

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
    if (status_ == WorldObject::STATUS_DYING) {
        status_ = WorldObject::STATUS_DEAD;
    }
	waiting_animation_ = false;
}

int Creature::GetAttackingAnimationIndex(float angle) {
    int degreeAngle = (int)((angle / PI) * 360);
    degreeAngle += 45;
    int animationIndex = degreeAngle / 90;
    return animationIndex % 8;
}

float Creature::GetAttackingAngle(Vector2D targetDirection) {
    Vector2D versor = Vector2D::Normalized(targetDirection);
    float radianAngle = acos(versor.x);
    if (versor.y > 0) {
        radianAngle = 2*PI - radianAngle;
    }
	return radianAngle;
}

}  // namespace sprite
