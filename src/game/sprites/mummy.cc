
#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"


#include "../scenes/world.h"
#include "../utils/imagefactory.h"
#include "mummy.h"
#include "projectile.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace framework;
using namespace scene;

namespace sprite {

#define MUMMY_WIDTH  74 
#define MUMMY_HEIGHT 74
#define PI acos(-1)
#define SQRT_3 1.7320508075688772935274463415059
#define EXP_PARAM (1.0)

// Devolve um tempo ~exp(EXP_PARAM)
static int WaitingTime () {

    return (int)(1000*-log(1.0*rand()/RAND_MAX)/EXP_PARAM);

}

Mummy::Mummy(Image* img) {
    if(img == NULL){
        utils::ImageFactory img_fac;
        img = img_fac.MummyImage();
    }
    Initialize(img);
    //Initialize(VIDEO_MANAGER()->LoadImage("data/images/mage_red_74x74.png"));

    World *world = ((World *)Engine::reference()->CurrentScene());
    directions_[Direction_::RIGHT] = world->FromScreenLinearCoordinates(Vector2D(SQRT_3/2, 0));
    directions_[Direction_::LEFT] =  world->FromScreenLinearCoordinates(Vector2D(-SQRT_3/2, 0));
    directions_[Direction_::DOWN] =  world->FromScreenLinearCoordinates(Vector2D(0, .5));
    directions_[Direction_::UP] =  world->FromScreenLinearCoordinates(Vector2D(0, -.5));

    // Animations
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

    this->SelectSpriteAnimation(last_standing_animation_, Vector2D(MUMMY_WIDTH, MUMMY_HEIGHT));
    set_hotspot(Vector2D(37, 55));
    collision_radius_ = 0.1f;
    is_attacking_ = false;
    speed_ = 2.0f;
    interval_ = new TimeAccumulator(0);


}


void Mummy::Think() {

    if (interval_->Expired()) {

        int dir = rand()%8;

        animation_direction_ = 0;
        if (dir < 3) animation_direction_ += Animation_::UP;
        if (dir >= 2 && dir < 5) animation_direction_ += Animation_::LEFT;
        if (dir >= 4 && dir < 7) animation_direction_ += Animation_::DOWN;
        if (dir >= 6 || dir == 0) animation_direction_ += Animation_::RIGHT;

        last_standing_animation_ = *(standing_animations_[animation_direction_]);

        this->walking_direction_ = Vector2D(cos(dir*PI/4), sin(dir*PI/4));

        interval_->Restart(WaitingTime());

    }

}

void Mummy::StartAttack() {
}

void Mummy::Update(float delta_t) {
    Creature::Update(delta_t);
    Vector2D dir(0,0);

    Think();
    is_attacking_ = false;

    if (!is_attacking_) {
        last_standing_animation_ = *(standing_animations_[animation_direction_]);
        if (animation_direction_ & Animation_::UP)
            dir = dir + directions_[Direction_::UP]; 
        if (animation_direction_ & Animation_::DOWN)
            dir = dir + directions_[Direction_::DOWN]; 
        if (animation_direction_ & Animation_::LEFT)
            dir = dir + directions_[Direction_::LEFT]; 
        if (animation_direction_ & Animation_::RIGHT)
            dir = dir + directions_[Direction_::RIGHT]; 

        Creature::Move(this->GetWalkingDirection(), delta_t);
        this->SelectSpriteAnimation(*walking_animations_[animation_direction_], Vector2D(MUMMY_WIDTH, MUMMY_HEIGHT));
    }

}
}
