#include <cmath>
#include <iostream>

#include <ugdk/base/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/time/timemanager.h>
#include <ugdk/time/timeaccumulator.h>
#include <ugdk/audio/audiomanager.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "game/sprites/creatures/hero.h"

#include "game/utils/imagefactory.h"
#include "game/sprites/item.h"
#include "game/sprites/creatures/mummy.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"

#include "game/scenes/world.h"

#include "game/skills/herobaseweapon.h"
#include "game/skills/usearguments.h"
#include "game/skills/skill.h"

#include <cmath>
#include <iostream>

#define INVUL_TIME 2000

using namespace std;
using namespace ugdk;
using namespace utils;
using resource::Resource;
using resource::Energy;
using resource::CountableResource;

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059

COLLISION_DIRECT(Hero*, MummySlowCollision, mummy) {
    data_->CollisionSlow();
}

Hero::Hero(ugdk::graphic::Spritesheet* img, 
           resource::Energy &life, 
           resource::Energy &mana, 
           int num_blocks, 
           double mana_per_block)
    : Creature(life, mana),
      mana_blocks_(mana_, num_blocks, mana_per_block)  {

    Initialize(img, ANIMATIONS);

    // Animations
    last_standing_animation_ = standing_animations_[Animation_::DOWN];
    identifier_ = "Hero";

    for (int i = 0; i < 4; i++) {
        pressed_key_[i] = false;
    }
    sprite_->SelectAnimation(last_standing_animation_);
    original_speed_ = speed_ = Constants::HERO_SPEED;

    invulnerability_time_ = INVUL_TIME;
    super_armor_ = true;

    slot_selected_ = -1;
    weapon_ = new skills::HeroBaseWeapon(this);
    secondary_weapon_ = NULL;

    light_oscilation_ = 0.0;
}

Hero::~Hero() {}

double Hero::FullMana() {
    return mana_blocks_.max_value() * Constants::HERO_MANA_PER_BLOCK;
}

void Hero::AddWeapon(int slot, skills::Skill* combat_art) {
    if (!weapons_.count(slot)) weapons_[slot] = combat_art;
    if (!secondary_weapon_) ChangeSecondaryWeapon(slot);
}

void Hero::ChangeSecondaryWeapon(int slot) {
    if (slot != slot_selected_) {
        slot_selected_ = slot;
        secondary_weapon_ = weapons_[slot];
    }
}

void Hero::PlayHitSound() const {
    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/hero_hit.wav")->Play();
}


void Hero::CollisionSlow() {
   speed_ /= 1.19;
}

void Hero::GetKeys() {
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();

    for (int i = 0; i < 4; i++) {
        pressed_key_[i] = false;
    }

    animation_direction_ = 0;
    int num_dirs = 0;
    if(input_->KeyDown(ugdk::input::K_w)) {
        pressed_key_[Direction_::UP] = true;
        animation_direction_ += Animation_::UP;
        num_dirs++;
    }
    if(input_->KeyDown(ugdk::input::K_a)) {
        pressed_key_[Direction_::LEFT] = true;
        animation_direction_ += Animation_::LEFT;
        num_dirs++;
    }
    if(input_->KeyDown(ugdk::input::K_s) && num_dirs < 2) {
        pressed_key_[Direction_::DOWN] = true;
        animation_direction_ += Animation_::DOWN;
        num_dirs++;
    }
    if(input_->KeyDown(ugdk::input::K_d) && num_dirs < 2) {
        pressed_key_[Direction_::RIGHT] = true;
        animation_direction_ += Animation_::RIGHT;
        num_dirs++;
    }

    if(weapons_.size() > 0) {
        if (input_->KeyPressed(ugdk::input::K_e)) {
            int next_slot = slot_selected_;
            do next_slot = (next_slot+1)%Constants::HERO_MAX_WEAPONS;
            while (!weapons_.count(next_slot));
            ChangeSecondaryWeapon(next_slot);
        }
        if (input_->KeyPressed(ugdk::input::K_q)) {
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

void Hero::StartAttackAnimation() {
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();

    Vector2D projectile_height(0, Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    Vector2D screen_center = Engine::reference()->window_size() * 0.5;

    double attackAngle = GetAttackingAngle(input_->GetMousePosition() - screen_center);
    int attackAnimationIndex = GetAttackingAnimationIndex(attackAngle);
    waiting_animation_ = true;
    last_standing_animation_ = Creature::standing_animations_[direction_mapping_[attackAnimationIndex]];
    sprite_->SelectAnimation(Creature::attacking_animations_[attackAnimationIndex]);
}

bool Hero::Aiming() {
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();
    return input_->MouseDown(ugdk::input::M_BUTTON_LEFT) || input_->MouseDown(ugdk::input::M_BUTTON_RIGHT);
}

bool Hero::ShootingWithWeapon() {
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();
    return input_->MouseDown(ugdk::input::M_BUTTON_LEFT) && weapon_ && weapon_->Available();
}

bool Hero::ShootingWithSecondaryWeapon() {
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();
    return input_->MouseDown(ugdk::input::M_BUTTON_RIGHT) && secondary_weapon_ && secondary_weapon_->Available();
}

void Hero::UpdateAim() {
    // Setting up the Aim resource and local variables.
    ugdk::input::InputManager *input = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);

    aim_destination_ = scene::World::FromScreenCoordinates(input->GetMousePosition() + projectile_height);
}

void Hero::Update(double delta_t) {
    Creature::Update(delta_t);
    if(is_active()) {
        if(Aiming()) {
            UpdateAim();
        }
        if(!waiting_animation_) {
            if (ShootingWithWeapon()) {
                if(weapon_->IsValidUse()) {
                    StartAttackAnimation();
                    weapon_->Use();
                }

            } else if (ShootingWithSecondaryWeapon()) {
                if(secondary_weapon_->IsValidUse()) {
                    StartAttackAnimation();
                    secondary_weapon_->Use();
                }

            }
        }
        if(!waiting_animation_) {
            this->GetKeys();
            Creature::Move(this->GetWalkingDirection(), delta_t);

            if (animation_direction_)
                sprite_->SelectAnimation(walking_animations_[animation_direction_]);
            else
                sprite_->SelectAnimation(last_standing_animation_);
        }
    }
    AdjustBlink(delta_t);
    speed_ = original_speed_;
    //mana_ += mana_regen_ * mana_regen_ratio_.Get() * delta_t;


    light_oscilation_ += delta_t;
    if(light_oscilation_ > 0.5) light_oscilation_ -= 0.5 * 2;

    if(light_oscilation_ < 0)
        this->set_light_radius(this->light_radius() - delta_t);
    else
        this->set_light_radius(this->light_radius() + delta_t);
}

void Hero::Invulnerable(int time) {
	this->hit_duration_->Restart(time);
}

void Hero::SetupCollision() {
    if(collision_object_) {
        delete collision_object_;
        collision_object_ = NULL;
    }
    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Hero);
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Circle(0.3));
    AddKnownCollisions();
}

void Hero::AddKnownCollisions() {
    Creature::AddKnownCollisions();
    ADD_COLLISIONLOGIC(Mummy, new MummySlowCollision(this));
}

}
