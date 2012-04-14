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

#include "game/components/hero.h"
#include "game/components/controller.h"

#include "game/utils/imagefactory.h"
#include "game/sprites/item.h"
#include "game/components/mummy.h"
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

namespace component {

#define SQRT_3 1.7320508075688772935274463415059

COLLISION_DIRECT(Hero*, MummySlowCollision, mummy) {
    data_->CollisionSlow();
}

Hero::Hero(sprite::WorldObject* owner,
           ugdk::graphic::Spritesheet* img, 
           resource::Energy &life, 
           resource::Energy &mana, 
           int num_blocks, 
           double mana_per_block)
    : Creature(owner, life, mana),
      mana_blocks_(mana_, num_blocks, mana_per_block)  {

    Initialize(img, ANIMATIONS);

    //owner->identifier_ = "Hero";
    sprite_->SelectAnimation(standing_animations_[last_standing_direction_.value()]);
    original_speed_ = speed_ = Constants::HERO_SPEED;

    /*
    TODO: argh
    invulnerability_time_ = INVUL_TIME;
    super_armor_ = true;*/

    slot_selected_ = -1;
    active_skills_[Controller::PRIMARY] = new skills::HeroBaseWeapon(this);
}

Hero::~Hero() {}

double Hero::FullMana() {
    return mana_blocks_.max_value() * Constants::HERO_MANA_PER_BLOCK;
}

void Hero::AddWeapon(int slot, skills::Skill* skill) {
    if (!skills_.count(slot)) skills_[slot] = skill;
    if (!active_skills_[Controller::SECONDARY]) ChangeSecondaryWeapon(slot);
}

void Hero::ChangeSecondaryWeapon(int slot) {
    if (slot != slot_selected_) {
        slot_selected_ = slot;
        active_skills_[Controller::SECONDARY] = skills_[slot];
    }
}

void Hero::PlayHitSound() const {
    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/hero_hit.wav")->Play();
}


void Hero::CollisionSlow() {
   speed_ /= 1.19;
}

void Hero::StartAttackAnimation() {
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();

    Vector2D projectile_height(0, Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);
    Vector2D screen_center = Engine::reference()->window_size() * 0.5;

    double attackAngle = GetAttackingAngle(input_->GetMousePosition() - screen_center);
    int attackAnimationIndex = GetAttackingAnimationIndex(attackAngle);
    waiting_animation_ = true;
    last_standing_direction_ = direction_mapping_[attackAnimationIndex];
    sprite_->SelectAnimation(Creature::attacking_animations_[attackAnimationIndex]);
}

bool Hero::Aiming() {
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();
    return input_->MouseDown(ugdk::input::M_BUTTON_LEFT) || input_->MouseDown(ugdk::input::M_BUTTON_RIGHT);
}

void Hero::UpdateAim() {
    // Setting up the Aim resource and local variables.
    ugdk::input::InputManager *input = Engine::reference()->input_manager();
    Vector2D projectile_height(0,Constants::PROJECTILE_SPRITE_HEIGHT+Constants::PROJECTILE_HEIGHT);

    aim_destination_ = scene::World::FromScreenCoordinates(input->GetMousePosition() + projectile_height);
}

void Hero::Update(double delta_t) {
    Creature::Update(delta_t);
    if(owner_->is_active()) {
        if(Aiming()) {
            UpdateAim();
        }
        component::Controller* controller = owner_->controller();

        if(!waiting_animation_) {
            std::map<Controller::SkillSlot, skills::Skill*>::iterator itsk;
            for(itsk = active_skills_.begin(); itsk != active_skills_.end(); ++itsk) {
                if(!itsk->second) continue;
                if(controller->IsUsingSkillSlot(itsk->first) && itsk->second->Available()) {
                    if(itsk->second->IsValidUse()) {
                        StartAttackAnimation();
                        itsk->second->Use();
                    }
                    break;
                }
            }
        }
        if(!waiting_animation_) {
            walking_direction_ = controller->direction_vector();
            const Direction& direction = controller->direction();
            if(direction) {
                last_standing_direction_ = direction;
                sprite_->SelectAnimation(walking_animations_[direction.value()]);
            } else {
                sprite_->SelectAnimation(standing_animations_[last_standing_direction_.value()]);
            }
            Creature::Move(this->GetWalkingDirection(), delta_t);
        }
    }
    speed_ = original_speed_;
}

void Hero::SetupCollision() {
    if(owner_->collision_object()) {
        delete owner_->collision_object();
        owner_->set_collision_object(NULL);
    }
    owner_->set_collision_object(new pyramidworks::collision::CollisionObject(WORLD()->collision_manager(), owner_));
    owner_->collision_object()->InitializeCollisionClass("Hero");
    owner_->collision_object()->set_shape(new pyramidworks::geometry::Circle(0.3));
    AddKnownCollisions();
}

void Hero::AddKnownCollisions() {
    Creature::AddKnownCollisions();
    owner_->collision_object()->AddCollisionLogic("Mummy", new MummySlowCollision(this));
}

}
