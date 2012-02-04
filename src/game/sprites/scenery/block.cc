
#include <cmath>
#include <ugdk/input/inputmanager.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <pyramidworks/geometry/rect.h>

#include "block.h"

#include "game/sprites/projectiles/projectile.h"
#include "game/sprites/scenery/wall.h"
#include "game/utils/constants.h"

#define BLOCK_MOVE_SPEED 2.0f

namespace sprite {

using namespace ugdk;
using namespace utils;

COLLISION_DIRECT(Block*, InvalidMovementCollision, obj) {
    data_->RevertPosition();
}

COLLISION_DIRECT(Block*, PushOnCollision, obj) {
    Vector2D pushdir = (((WorldObject *)obj)->world_position() - data_->world_position()).Normalize();
    data_->PushToward(pushdir);
}

Block::Block(ugdk::graphic::FlexibleSpritesheet* image) : moving_(false) {
    image->set_hotspot(Vector2D(Constants::WALL_HOTSPOT_X, Constants::WALL_HOTSPOT_Y));

    ugdk::graphic::Sprite* sprite = new ugdk::graphic::Sprite(image);
    node_->set_drawable(sprite);
    node_->modifier()->set_scale(Vector2D(1.0f, 0.7f)); // TODO make block offset

    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Block);
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Rect(0.95f, 0.95f));
    ADD_COLLISIONLOGIC(Wall, new InvalidMovementCollision(this));
    ADD_COLLISIONLOGIC(Projectile, new PushOnCollision(this));
}
Block::~Block() {}

#ifdef DEBUG
void Block::GetKeys() {
    ugdk::input::InputManager *input = INPUT_MANAGER();
    moving_ = true;
    moving_time_left_ = 0.5f;
    if(input->KeyDown(ugdk::input::K_UP)) {
        moving_toward_ = UP;
    } else if(input->KeyDown(ugdk::input::K_DOWN)) {
        moving_toward_ = DOWN;
    } else if(input->KeyDown(ugdk::input::K_RIGHT)) {
        moving_toward_ = RIGHT;
    } else if(input->KeyDown(ugdk::input::K_LEFT)) {
        moving_toward_ = LEFT;
    } else {
        moving_ = false;
    }
}
#endif

void Block::MoveBlock(float delta_t) {
    moving_time_left_ -= delta_t;
    Vector2D newpos = this->world_position();
    switch(moving_toward_) {
    case RIGHT:
        newpos.x -= BLOCK_MOVE_SPEED * delta_t;
        break;
    case LEFT:
        newpos.x += BLOCK_MOVE_SPEED * delta_t;
        break;
    case UP:
        newpos.y -= BLOCK_MOVE_SPEED * delta_t;
        break;
    case DOWN:
        newpos.y += BLOCK_MOVE_SPEED * delta_t;
        break;
    }
    set_world_position(newpos);
    if(moving_time_left_ <= 0.0f) {
        moving_ = false;
        last_stable_position_ = world_position();
    }
}

void Block::Update(float delta_t) {
    WorldObject::Update(delta_t);
    if(moving_) {
        MoveBlock(delta_t);
    } else {
        last_stable_position_ = world_position();
#ifdef DEBUG
        GetKeys();
#endif
    }
}

void Block::PushToward(Vector2D &pushdir) {
    if(moving_) return; // One cannot affect a block while it moves.

    if(pushdir.x > fabs(pushdir.y)) 
        moving_toward_ = RIGHT;
    else if (pushdir.x < -fabs(pushdir.y))
        moving_toward_ = LEFT;
    else if (pushdir.y > fabs(pushdir.x))
        moving_toward_ = UP;
    else
        moving_toward_ = DOWN;

    moving_ = true;
    moving_time_left_ = 0.5f;
}

void Block::RevertPosition() {
    this->set_world_position(last_stable_position_);
    moving_ = false;
}

}
