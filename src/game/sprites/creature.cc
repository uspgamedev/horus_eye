#include "../../framework/animation.h"
#include "../../framework/animationset.h"
#include "../../framework/animationparser.h"
#include "../../framework/engine.h"
#include "creature.h"
#include "worldobject.h"
#include "mummy.h"
#include "hero.h"
#include "door.h"
#include "wall.h"
#include "../utils/circleobject.h"
#include "../scenes/world.h"

#include <cmath>
#include <iostream>

using namespace ugdk;
using namespace utils;

#define PI acos(-1.0f)

namespace sprite {

int Creature::direction_mapping_[8];
uint32 Creature::standing_animations_[16];
uint32 Creature::walking_animations_[16];
uint32 Creature::attacking_animations_[8];
uint32 Creature::taking_damage_animation_ = -1;
uint32 Creature::dying_animation_ = -1;
Vector2D Creature::directions_[4];

AnimationSet* Creature::ANIMATIONS = NULL;

Creature::Creature() : WorldObject() {
    waiting_animation_ = false;
    weapon_ = NULL;
	last_stable_position_ = Vector2D(0,0);
    mana_regen_ = 0.0f;
    sight_count_ = 0;
    super_armor_ = false;
    invulnerability_time_ = 0;
    blink_ = false;

    blink_time_ = new TimeAccumulator(75);
    hit_duration_ = new TimeAccumulator(0);

    collision_type_ = MOVEABLE;
}

Creature::~Creature() {
    if (hit_duration_) delete hit_duration_;
    if (blink_time_) delete blink_time_;
}

void Creature::Initialize(Drawable *image, AnimationSet *set,
                          bool delete_image) {
    Sprite::Initialize(image, set, delete_image);
    AddObserverToAnimation(this);
}


bool deletecondition(Condition *condition) {
	bool is_finished = (condition->phase() == Condition::PHASE_FINISHED);
	if (is_finished) delete condition;
    return is_finished;
}

bool Creature::AddCondition(Condition* new_condition) {
    conditions_.push_front(new_condition);
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
            blink_time_->Restart();
        }
    } else 
        blink_ = false;
}

void Creature::TakeDamage(float life_points) {
    if(!hit_duration_->Expired()) return;
    fprintf(stderr, "Decreasing life of %d from %f to %f (dmg = %f)\n", (int) this, life_, life_ - life_points, life_points);
    PlayHitSound();
    life_ -= life_points;
    if(life_ <= 0.0f) {
        if (status_ == WorldObject::STATUS_ACTIVE) {
            this->SelectAnimation(dying_animation_);
            this->status_ = WorldObject::STATUS_DYING;
            this->collision_type_ = WorldObject::NO_COLLISION;
        }
    } else if(!super_armor_) {
        waiting_animation_ = true;
        this->SelectAnimation(taking_damage_animation_);
    }
    hit_duration_->Restart(invulnerability_time_);
    blink_time_->Restart();
}

void Creature::InitializeAnimations() {
    if (ANIMATIONS == NULL) {
        ANIMATIONS = Engine::reference()->animation_parser()->Load("data/animations/creature.and");
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
}

void Creature::ReleaseAnimations() {
    ANIMATIONS = NULL;
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
    for (int i = 0; i < 16; i++)
        walking_animations_[i] = -1;
    walking_animations_[Animation_::DOWN] = ANIMATIONS->MakeIndex("WALKING_DOWN");
    walking_animations_[Animation_::LEFT] = ANIMATIONS->MakeIndex("WALKING_LEFT");
    walking_animations_[Animation_::RIGHT] = ANIMATIONS->MakeIndex("WALKING_RIGHT");
    walking_animations_[Animation_::UP] = ANIMATIONS->MakeIndex("WALKING_UP");
    walking_animations_[Animation_::DOWN | Animation_::RIGHT] =
            ANIMATIONS->MakeIndex("WALKING_DOWN_RIGHT");
    walking_animations_[Animation_::DOWN | Animation_::LEFT] =
            ANIMATIONS->MakeIndex("WALKING_DOWN_LEFT");
    walking_animations_[Animation_::UP | Animation_::RIGHT] =
            ANIMATIONS->MakeIndex("WALKING_UP_RIGHT");
    walking_animations_[Animation_::UP | Animation_::LEFT] =
            ANIMATIONS->MakeIndex("WALKING_UP_LEFT");
}

void Creature::InitializeStandingAnimations() {
    for (int i = 0; i < 16; i++)
        standing_animations_[i] = -1;
    ANIMATIONS->Add("STANDING_DOWN", 4, -1);
    standing_animations_[Animation_::DOWN] =
            ANIMATIONS->MakeIndex("STANDING_DOWN");
    ANIMATIONS->Add("STANDING_LEFT", 7, -1);
    standing_animations_[Animation_::LEFT] =
            ANIMATIONS->MakeIndex("STANDING_LEFT");
    ANIMATIONS->Add("STANDING_RIGHT", 2, -1);
    standing_animations_[Animation_::RIGHT] =
            ANIMATIONS->MakeIndex("STANDING_RIGHT");
    ANIMATIONS->Add("STANDING_UP", 0, -1);
    standing_animations_[Animation_::UP] =
            ANIMATIONS->MakeIndex("STANDING_UP");
    ANIMATIONS->Add("STANDING_DOWN_RIGHT", 3, -1);
    standing_animations_[Animation_::DOWN | Animation_::RIGHT] =
            ANIMATIONS->MakeIndex("STANDING_DOWN_RIGHT");
    ANIMATIONS->Add("STANDING_DOWN_LEFT", 6, -1);
    standing_animations_[Animation_::DOWN | Animation_::LEFT] =
            ANIMATIONS->MakeIndex("STANDING_DOWN_LEFT");
    ANIMATIONS->Add("STANDING_UP_RIGHT", 1, -1);
    standing_animations_[Animation_::UP | Animation_::RIGHT] =
            ANIMATIONS->MakeIndex("STANDING_UP_RIGHT");
    ANIMATIONS->Add("STANDING_UP_LEFT", 8, -1);
    standing_animations_[Animation_::UP | Animation_::LEFT] =
            ANIMATIONS->MakeIndex("STANDING_UP_LEFT");
}

void Creature::Move(Vector2D direction, float delta_t) {
    Vector2D position(this->world_position().x, this->world_position().y);
    last_stable_position_ = position;
    position = position + direction * (this->speed_ * delta_t);
    set_world_position(position);
}

void Creature::CollideWithRect(const RectObject *rect) {

    set_world_position(last_stable_position_);

    const CircleObject *circle = (const CircleObject*)bound_;

    Vector2D line(rect->width(), rect->height());
    Vector2D circ_pos = circle->position();
    Vector2D rect_pos = rect->position();

    float left = rect_pos.x - line.x/2;
    float bottom = rect_pos.y - line.y/2;
    float right = left + line.x;
    float top = bottom + line.y;

    if (circ_pos.y < top && circ_pos.y > bottom)
        walking_direction_.x = 0;
    else if (circ_pos.x < right && circ_pos.x > left)
        walking_direction_.y = 0;
    else {
        Vector2D point(left, bottom);
        if (circ_pos.x > right) point.x  = right;
        if (circ_pos.y > top)   point.y = top;
        Vector2D dir = circ_pos - point,
                 tg_dir(-dir.y, dir.x);
        if (Vector2D::InnerProduct(walking_direction_, tg_dir) > 0)
            walking_direction_ = tg_dir;
        else
            walking_direction_ = Vector2D()-tg_dir;
    }

    walking_direction_ = Vector2D::Normalized(walking_direction_);

}

void Creature::Tick() {
    if (status_ == WorldObject::STATUS_DYING) {
        status_ = WorldObject::STATUS_DEAD;
    }
	waiting_animation_ = false;
}

void Creature::CollidesWith(Wall * obj) {
    const RectObject *rect = (const RectObject*)obj->bound();
    CollideWithRect(rect);
}

void Creature::CollidesWith(Door * obj) {
    const RectObject *rect = (const RectObject*)obj->bound();
    CollideWithRect(rect);
}

void Creature::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
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
    printf("%f\n", radianAngle);
	return radianAngle;
}

void Creature::Render() {
    if (!blink_) WorldObject::Render();
}

}  // namespace sprite
