
#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"

#include "../scenes/world.h"
#include "hero.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace framework;
using namespace scene;

namespace sprite {

#define HERO_WIDTH  74 
#define HERO_HEIGHT 74
#define PI acos(-1)

#define SQRT_3 1.7320508075688772935274463415059
 
Hero::Hero() {

    Initialise(VIDEO_MANAGER()->LoadImage("data/images/mage_74x74.png"));


    World *world = ((World *)Engine::reference()->CurrentScene());
    directions_[Direction_::RIGHT] = world->FromScreenLinearCoordinates(Vector2D(SQRT_3/2, 0));
    directions_[Direction_::LEFT] =  world->FromScreenLinearCoordinates(Vector2D(-SQRT_3/2, 0));
    directions_[Direction_::DOWN] =  world->FromScreenLinearCoordinates(Vector2D(0, .5));
    directions_[Direction_::UP] =  world->FromScreenLinearCoordinates(Vector2D(0, -.5));

    is_attacking_ = false;

    last_standing_animation_ = new Animation(0, -1);
    for (int i = 0; i < 16; i++) {
        standing_animations_[i] = (Animation **) malloc (sizeof (Animation *));
        walking_animations_[i] = (Animation **) malloc (sizeof (Animation *));
        *standing_animations_[i] = NULL;
        *walking_animations_[i] = NULL;
    }
    *standing_animations_[Animation_::DOWN] = new Animation(0, 4, -1);
    *standing_animations_[Animation_::LEFT] = new Animation(0, 7, -1);
    *standing_animations_[Animation_::RIGHT] = new Animation(0, 2, -1);
    *standing_animations_[Animation_::UP] = new Animation(0, 0, -1);
    *standing_animations_[Animation_::DOWN | Animation_::RIGHT] = new Animation(0, 3, -1);
    *standing_animations_[Animation_::DOWN | Animation_::LEFT] = new Animation(0, 6, -1);
    *standing_animations_[Animation_::UP | Animation_::RIGHT] = new Animation(0, 1, -1);
    *standing_animations_[Animation_::UP | Animation_::LEFT] = new Animation(0, 8, -1);

    *walking_animations_[Animation_::DOWN] = new Animation(10, 4, 14, 24, 34, 44, -1);
    *walking_animations_[Animation_::LEFT] = new Animation(10, 7, 17, 27, 37, 47, -1);
    *walking_animations_[Animation_::RIGHT] = new Animation(10, 2, 12, 22, 32, 42, -1);
    *walking_animations_[Animation_::UP] = new Animation(10, 0, 10, 20, 30, 40, -1);
    *walking_animations_[Animation_::DOWN | Animation_::RIGHT] = new Animation(10, 3, 13, 23, 33, 43, -1);
    *walking_animations_[Animation_::DOWN | Animation_::LEFT] = new Animation(10, 6, 16, 26, 36, 46, -1);
    *walking_animations_[Animation_::UP | Animation_::RIGHT] = new Animation(10, 1, 11, 21, 31, 41, -1);
    *walking_animations_[Animation_::UP | Animation_::LEFT] = new Animation(10, 8, 18, 28, 38, 48, -1);

    attacking_animations_[6] = new Animation(10, 54, 64, 74, 84, -1);
    attacking_animations_[4] = new Animation(10, 57, 67, 77, 87, -1);
    attacking_animations_[0] = new Animation(10, 52, 62, 72, 82, -1);
    attacking_animations_[2] = new Animation(10, 50, 60, 70, 80, -1);
    attacking_animations_[7] = new Animation(10, 53, 63, 73, 83, -1);
    attacking_animations_[5] = new Animation(10, 56, 66, 76, 86, -1);
    attacking_animations_[1] = new Animation(10, 51, 61, 71, 81, -1);
    attacking_animations_[3] = new Animation(10, 58, 68, 78, 88, -1);

    screen_center_ = Engine::reference()->window_size() * .5;

    for (int i = 0; i < 8; i++) {
        attacking_animations_[i]->AddObserver(this);
    }
    
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
    for (int i = 0; i < 16; i++) {
        if (*standing_animations_[i] == NULL) {
            free(standing_animations_[i]);
            standing_animations_[i] = &last_standing_animation_;
        }
        if (*walking_animations_[i] == NULL) {
            free(walking_animations_[i]);
            walking_animations_[i] = &last_standing_animation_;
        }
    }

    for (int i = 0; i < 4; i++) {
        pressed_key_[i] = false;
    }
    SelectSpriteAnimation(last_standing_animation_, Vector2D(HERO_WIDTH, HERO_HEIGHT));

}

void Hero::Tick() {
    is_attacking_ = false;
}

void Hero::SelectSpriteAnimation(Animation *animation, Vector2D frame_size) {
    this->image()->set_frame_size(frame_size);
    this->SelectAnimation(animation);
}

void Hero::Move(float delta_t) {
    float speed = 4*delta_t;

    Vector2D position(this->world_position().x, this->world_position().y);
    Vector2D dir (0, 0);
    for (int i = 0; i < 4; i++) {
        if (pressed_key_[i]) {
            dir = dir + directions_[i];
        }
    }
    dir = Vector2D::Normalized(dir);
    dir = dir * speed;
    position = position + dir;

    set_world_position(position);
}

void Hero::GetKeys() {
    InputManager *input_ = Engine::reference()->input_manager();
    // -----------------------------
    // Nao copiar esta parte
    if(input_->KeyDown(K_ESCAPE))
        Engine::reference()->quit();
    //        if(input_->KeyDown(SDLK_SPACE))
    //            mage_->Spell();
    // -----------------------------

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
}

void Hero::GetMouseState() {
    InputManager *input_ = Engine::reference()->input_manager();
    if (input_->MouseDown(M_BUTTON_LEFT) && !is_attacking_) {
        is_attacking_ = true;
        int attackAnimationIndex = GetAttackingAnimationIndex(input_->GetMousePosition());
        last_standing_animation_ = *standing_animations_[direction_mapping_[attackAnimationIndex]];
        this->SelectSpriteAnimation(attacking_animations_[attackAnimationIndex], Vector2D(HERO_WIDTH, HERO_HEIGHT));
    }
}

int Hero::GetAttackingAnimationIndex(Vector2D mousePosition) {
    Vector2D versor = Vector2D::Normalized(mousePosition - screen_center_);
    double radianAngle = acos(versor.x);
    if (versor.y > 0) {
        radianAngle = 2*PI - radianAngle;
    }

    int degreeAngle = (int)((radianAngle / PI) * 360);
    
    degreeAngle += 45;

    int animationIndex = degreeAngle / 90;
    return animationIndex % 8;
}

void Hero::Update(float delta_t) {
    Creature::Update(delta_t);
    if (!is_attacking_) {
        this->GetKeys();
        this->Move(delta_t);
        this->SelectSpriteAnimation(*walking_animations_[animation_direction_], Vector2D(HERO_WIDTH, HERO_HEIGHT));
        this->GetMouseState();
    }

}
}
