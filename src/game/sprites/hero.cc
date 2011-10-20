#include <cmath>
#include <iostream>

#include <ugdk/base/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/animation.h>
#include <ugdk/action/sprite.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/time/timehandler.h>
#include <ugdk/audio/audiomanager.h>

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
#include "../utils/settings.h"
#include "weapons/herobaseweapon.h"
#include <cmath>
#include <iostream>

#include <assert.h>

using namespace std;
using namespace ugdk;
using namespace scene;
using namespace utils;

namespace sprite {

INITIALIZE_COLLIDABLE_NODE(Hero, Creature);

#define SQRT_3 1.7320508075688772935274463415059

#define HERO_HOTSPOT_X Constants::HERO_HOTSPOT_X
#define HERO_HOTSPOT_Y Constants::HERO_HOTSPOT_Y

Hero::Hero(Image* img) {
    if(img == NULL){
        utils::ImageFactory img_fac;
        img = img_fac.HeroImage();
    }
    Initialize(img, ANIMATIONS);

    // Animations
    screen_center_ = Engine::reference()->window_size() * .5;

    animation_direction_ = 0;
    last_standing_animation_ = standing_animations_[Animation_::DOWN];

    for (int i = 0; i < 4; i++) {
        pressed_key_[i] = false;
    }
    SelectAnimation(last_standing_animation_);
    set_hotspot(Vector2D(static_cast<float>(HERO_HOTSPOT_X),
						 static_cast<float>(HERO_HOTSPOT_Y)));
    original_speed_ = speed_ = Constants::HERO_SPEED;

    // Initializing life and mana
    life_ = max_life_ = Constants::HERO_MAX_LIFE;
    mana_ = max_mana_ = Constants::HERO_MAX_MANA;
    mana_regen_ = Constants::HERO_MANA_REGEN;
    set_light_radius(Constants::LIGHT_RADIUS_INITIAL);
    bound_ = new CircleObject(0.3f);
    invulnerability_time_ = 2000;
    super_armor_ = true;

    slot_selected_ = -1;
    weapon_ = new HeroBaseWeapon(this);
    secondary_weapon_ = NULL;
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

void Hero::PlayHitSound() const {
    Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample("data/samples/hero_hit.wav")->Play();
}


void Hero::CollidesWith(Mummy *obj) {
   speed_ /= 1.19f;
}

void Hero::CollidesWith(MummyProjectile* obj) {
    TakeDamage(obj->damage());
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

    if(weapons_.size() > 0) {
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
    }

    if (animation_direction_)
        last_standing_animation_ = standing_animations_[animation_direction_];


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
    float attackAngle = GetAttackingAngle(input_->GetMousePosition() -
            screen_center_ + projectile_height);
    int attackAnimationIndex = GetAttackingAnimationIndex(attackAngle);
    waiting_animation_ = true;
    last_standing_animation_ = Creature::standing_animations_[direction_mapping_[attackAnimationIndex]];
    this->SelectAnimation(Creature::attacking_animations_[attackAnimationIndex]);
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
            if (animation_direction_)
                this->SelectAnimation(walking_animations_[animation_direction_]);
            else
                this->SelectAnimation(last_standing_animation_);
        }
    }
    AdjustBlink(delta_t);
    speed_ = original_speed_;
    set_mana(mana() + mana_regen_ * delta_t);
}

void Hero::Invulnerable(int time) {
	this->hit_duration_->Restart(time);
}

}
