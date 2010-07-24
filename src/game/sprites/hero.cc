
#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"

#include "../scenes/world.h"
#include "../utils/imagefactory.h"
#include "../utils/circleobject.h"
#include "hero.h"
#include "projectile.h"
#include "mummy.h"
#include "../utils/constants.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace framework;
using namespace scene;
using namespace utils;

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059

#define HERO_WIDTH  Constants::HERO_WIDTH
#define HERO_HEIGHT Constants::HERO_HEIGHT
#define HERO_HOTSPOT_X Constants::HERO_HOTSPOT_X
#define HERO_HOTSPOT_Y Constants::HERO_HOTSPOT_Y
#define MAX_LIFE Constants::HERO_MAX_LIFE
#define HERO_SPEED Constants::HERO_SPEED

Hero::Hero(Image* img) {
    if(img == NULL){
        utils::ImageFactory img_fac;
        img = img_fac.HeroImage();
    }
    Initialize(img);

    directions_[Direction_::RIGHT] = Vector2D(1, -1);
    directions_[Direction_::LEFT] = Vector2D(-1, 1);
    directions_[Direction_::DOWN] =  Vector2D(-1, -1);
    directions_[Direction_::UP] = Vector2D(1, 1);

    // Animations
    //last_standing_animation_ = new Animation(0, -1);
    InitializeStandingAnimations();
    InitializeWalkingAnimations();
    InitializeAttackingAnimations();

    dying_animation_ = new Animation(10, 90, 91, 92, 93, 94, 100, 101, -1);

    screen_center_ = Engine::reference()->window_size() * .5;

    dying_animation_->AddObserver(this);

    direction_mapping_[0] = Animation_::RIGHT;
    direction_mapping_[1] = Animation_::RIGHT | Animation_::UP;
    direction_mapping_[2] = Animation_::UP;
    direction_mapping_[3] = Animation_::UP | Animation_::LEFT;
    direction_mapping_[4] = Animation_::LEFT;
    direction_mapping_[5] = Animation_::LEFT | Animation_::DOWN;
    direction_mapping_[6] = Animation_::DOWN;
    direction_mapping_[7] = Animation_::DOWN | Animation_::RIGHT;

    animation_direction_ = 0;
    last_standing_animation_ = *standing_animations_[Animation_::DOWN];

    for (int i = 0; i < 4; i++) {
        pressed_key_[i] = false;
    }
    SelectSpriteAnimation(last_standing_animation_, Vector2D(HERO_WIDTH, HERO_HEIGHT));
    set_hotspot(Vector2D(HERO_HOTSPOT_X, HERO_HOTSPOT_Y));
    is_attacking_ = false;
    speed_ = HERO_SPEED;
    life_ = max_life_ = MAX_LIFE;
    hit_duration_ = new TimeAccumulator(0);
    bound_ = new CircleObject(0.3f);
    blink_time_ = 0;
    blink_ = false;
}

void Hero::CollidesWith(Mummy *obj) {
    if(obj->is_attacking() && hit_duration_->Expired()) {
        --life_;
        hit_duration_->Restart(2000);
        blink_time_ = 0;
        Engine::reference()->audio_manager()->LoadSample("data/samples/hero_hit.wav")->Play();
    }
    if(life_ <= 0)
        if (status_ == WorldObject::STATUS_ACTIVE) {
            this->SelectSpriteAnimation(dying_animation_, Vector2D(HERO_WIDTH, HERO_HEIGHT));
            this->status_ = WorldObject::STATUS_DYING;
        }
}

void Hero::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

void Hero::GetKeys() {
    InputManager *input_ = Engine::reference()->input_manager();

    for (int i = 0; i < 4; i++) {
        pressed_key_[i] = false;
    }

    animation_direction_ = 0;
    if(input_->KeyDown(K_w)) {
        pressed_key_[Direction_::UP] = true;
        animation_direction_ += Animation_::UP;
    }
    if(input_->KeyDown(K_a)) {
        pressed_key_[Direction_::LEFT] = true;
        animation_direction_ += Animation_::LEFT;
    }
    if(input_->KeyDown(K_s)) {
        pressed_key_[Direction_::DOWN] = true;
        animation_direction_ += Animation_::DOWN;
    }
    if(input_->KeyDown(K_d)) {
        pressed_key_[Direction_::RIGHT] = true;
        animation_direction_ += Animation_::RIGHT;
    }

    last_standing_animation_ = *(standing_animations_[animation_direction_]);


    Vector2D dir (0, 0);
    for (int i = 0; i < 4; i++) {
        if (pressed_key_[i]) {
            dir = dir + directions_[i];
        }
    }
    this->walking_direction_ = Vector2D::Normalized(dir);
}

void Hero::StartAttack() {
    InputManager *input_ = Engine::reference()->input_manager();

    double attackAngle = GetAttackingAngle(input_->GetMousePosition() - screen_center_);
    int attackAnimationIndex = GetAttackingAnimationIndex(attackAngle);
    is_attacking_ = true;
    last_standing_animation_ = *standing_animations_[direction_mapping_[attackAnimationIndex]];
    this->SelectSpriteAnimation(attacking_animations_[attackAnimationIndex], Vector2D(HERO_WIDTH, HERO_HEIGHT));

    World *world_ = ((World *)Engine::reference()->CurrentScene());
    // Ajuste da altura do projetil.
    Vector2D mouseOffset = input_->GetMousePosition() - screen_center_ + Vector2D(0,Constants::PROJECTILE_HEIGHT);
    Vector2D versor = world_->FromScreenLinearCoordinates(Vector2D::Normalized(mouseOffset)),
             pos = world_position();
    Projectile * projectile = new Projectile(pos, versor);
    world_->AddWorldObject(projectile);
    Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
}

void Hero::GetMouseState() {
    InputManager *input_ = Engine::reference()->input_manager();
    if (input_->MouseDown(M_BUTTON_LEFT) && !is_attacking_)
        StartAttack();
}

void Hero::Update(float delta_t) {
    Creature::Update(delta_t);
    if (!is_attacking_ && status_ == WorldObject::STATUS_ACTIVE) {
        Creature::Move(this->GetWalkingDirection(), delta_t);
        this->GetKeys();
        this->SelectSpriteAnimation(*walking_animations_[animation_direction_], Vector2D(HERO_WIDTH, HERO_HEIGHT));
        this->GetMouseState();
    }
    if (!hit_duration_->Expired()) {
    	blink_time_ += delta_t;
    	if (blink_time_ > 0.05) {
    		blink_ = !blink_;
    		blink_time_ = 0;
    	}
    }
    else if (blink_) {
    	blink_ = false;
    }

}

void Hero::Render(Image *back_buffer, Vector2D &offset) {

	if (!blink_) Sprite::Render(back_buffer, offset);

}

}
