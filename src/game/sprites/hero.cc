#include <cmath>
#include <iostream>

#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/animation.h"
#include "../../framework/sprite.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"
#include "../../framework/audiomanager.h"

#include "../scenes/world.h"
#include "../utils/imagefactory.h"
#include "../utils/circleobject.h"
#include "item.h"
#include "hero.h"
#include "projectile.h"
#include "explosion.h"
#include "mummyprojectile.h"
#include "mummy.h"
#include "../utils/constants.h"
#include "weapons/herobaseweapon.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace framework;
using namespace scene;
using namespace utils;

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059

#define HERO_HOTSPOT_X Constants::HERO_HOTSPOT_X
#define HERO_HOTSPOT_Y Constants::HERO_HOTSPOT_Y
#define MAX_LIFE Constants::HERO_MAX_LIFE
#define MAX_MANA Constants::HERO_MAX_MANA

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
    InitializeStandingAnimations();
    InitializeWalkingAnimations();
    InitializeAttackingAnimations();

    dying_animation_ = new Animation(10, 80, 81, 82, 83, 84, 90, 91, -1);

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
    SelectAnimation(last_standing_animation_);
    set_hotspot(Vector2D(static_cast<float>(HERO_HOTSPOT_X),
						 static_cast<float>(HERO_HOTSPOT_Y)));
    original_speed_ = speed_ = Constants::HERO_SPEED;
    life_ = max_life_ = MAX_LIFE;
    mana_ = max_mana_ = MAX_MANA;
    light_radius_ = 5.0f;
    hit_duration_ = new TimeAccumulator(0);
    bound_ = new CircleObject(0.3f);
    blink_time_ = 0;
    blink_ = false;
    slot_selected_ = -1;
    weapon_ = new HeroBaseWeapon(this);
    secondary_weapon_ = NULL;

    // Create the bigger fog image now, so the game doesn't hang during play.
    for (int i = 1; i <= 5; ++i)
        World::CreateFogTransparency(light_radius_ + i*Constants::SIGHT_POTION_INCREASE);
}

void Hero::AddWeapon(int slot, Weapon* weapon) {
    if (!weapons_.count(slot)) weapons_[slot] = weapon;
    if (!secondary_weapon_) ChangeSecondaryWeapon(slot);
}

void Hero::ChangeSecondaryWeapon(int slot) {
    if (slot != slot_selected_) {
        slot_selected_ = slot;
        secondary_weapon_ = weapons_[slot];
    }
}

void Hero::TakeDamage(int life_points) {
    if(hit_duration_->Expired()) {
        Creature::TakeDamage(life_points);
        Engine::reference()->audio_manager()->LoadSample("data/samples/hero_hit.wav")->Play();
        hit_duration_->Restart(2000);
        blink_time_ = 0;
    }
}

void Hero::CollidesWith(Mummy *obj) {
   speed_ /= 1.19f;
}

void Hero::CollidesWith(MummyProjectile* obj) {
    TakeDamage(obj->damage());
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
    int num_dirs = 0;
    if(input_->KeyDown(K_w)) {
        pressed_key_[Direction_::UP] = true;
        animation_direction_ += Animation_::UP;
        num_dirs++;
    }
    if(input_->KeyDown(K_a)) {
        pressed_key_[Direction_::LEFT] = true;
        animation_direction_ += Animation_::LEFT;
        num_dirs++;
    }
    if(input_->KeyDown(K_s) && num_dirs < 2) {
        pressed_key_[Direction_::DOWN] = true;
        animation_direction_ += Animation_::DOWN;
        num_dirs++;
    }
    if(input_->KeyDown(K_d) && num_dirs < 2) {
        pressed_key_[Direction_::RIGHT] = true;
        animation_direction_ += Animation_::RIGHT;
        num_dirs++;
    }
    if (input_->KeyPressed(K_e)) {
        int next_slot = slot_selected_;
        do next_slot = (next_slot+1)%Constants::HERO_MAX_WEAPONS;
        while (!weapons_.count(next_slot));
        ChangeSecondaryWeapon(next_slot);
    }
    if (input_->KeyPressed(K_q)) {
        int next_slot = slot_selected_;
        do next_slot = next_slot-1 < 0 ? Constants::HERO_MAX_WEAPONS-1 : next_slot-1;
        while (!weapons_.count(next_slot));
        ChangeSecondaryWeapon(next_slot);
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

    Vector2D projectile_height(0, Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    double attackAngle = GetAttackingAngle(input_->GetMousePosition() -
            screen_center_ + projectile_height);
    int attackAnimationIndex = GetAttackingAnimationIndex(attackAngle);
    waiting_animation_ = true;
    last_standing_animation_ = *standing_animations_[direction_mapping_[attackAnimationIndex]];
    this->SelectAnimation(attacking_animations_[attackAnimationIndex]);
}

void Hero::StartExplosion() {
    StartAttack();
}

bool Hero::ShootingWithWeapon() {
    InputManager *input_ = Engine::reference()->input_manager();
    return input_->MouseDown(M_BUTTON_LEFT) && weapon_ && weapon_->Available();
}

bool Hero::ShootingWithSecondaryWeapon() {
    InputManager *input_ = Engine::reference()->input_manager();
    return input_->MouseDown(M_BUTTON_RIGHT) && secondary_weapon_ && secondary_weapon_->Available();
}

void Hero::AdjustBlink(float delta_t) {
    if (!hit_duration_->Expired()) {
        blink_time_ += delta_t;
        if (blink_time_ > 0.05) {
            blink_ = !blink_;
            blink_time_ = 0;
        }
    } else if (blink_) {
        blink_ = false;
    }
}

void Hero::Update(float delta_t) {
    Creature::Update(delta_t);
    if (!waiting_animation_ && status_ == WorldObject::STATUS_ACTIVE) {
        if (ShootingWithWeapon()) {
            weapon_->Attack();
        } else if (ShootingWithSecondaryWeapon()) {
            secondary_weapon_->Attack();
        } else {
            Creature::Move(this->GetWalkingDirection(), delta_t);
            this->GetKeys();
            this->SelectAnimation(*walking_animations_[animation_direction_]);
        }
    }
    AdjustBlink(delta_t);
    speed_ = original_speed_;
}

}
