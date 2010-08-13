//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/lifepotion.cc
// lifepotion.cc da classe LifePotion
//

#include "lifepotion.h"
#include "../../framework/engine.h"
#include "../scenes/world.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"

#define CENTER_X    Constants::POTION_SPRITE_CENTER_X
#define CENTER_Y    Constants::POTION_SPRITE_CENTER_Y
#define HEIGHT      Constants::POTION_HEIGHT
#define POTION_WIDTH    Constants::POTION_SPRITE_WIDTH
#define POTION_HEIGHT   Constants::POTION_SPRITE_HEIGHT
#define POTION_RECOVER_LIFE   Constants::POTION_RECOVER_LIFE

namespace sprite {

LifePotion::LifePotion() {
    Initialize(VIDEO_MANAGER()->LoadImage("data/images/fire_ball.png"));
    image()->set_frame_size(Vector2D(POTION_WIDTH,POTION_HEIGHT));
    recover_life_ = POTION_RECOVER_LIFE;
    set_hotspot(Vector2D(CENTER_X, CENTER_Y + HEIGHT));
    this->bound_ = new CircleObject(0.15f);
    this->collision_type_ = STATIC;
    // TODO Auto-generated constructor stub

}

LifePotion::~LifePotion () {
    // TODO Auto-generated destructor stub
}

void LifePotion::CollidesWith(Hero *obj) {
    this->status_ = WorldObject::STATUS_DEAD;
}

void LifePotion::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
