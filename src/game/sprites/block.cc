
#include "block.h"

#include "game/sprites/projectile.h"
#include "game/utils/rectobject.h"
#include "game/utils/constants.h"

#define BLOCK_MOVE_SPEED 2.0f

namespace sprite {

using namespace ugdk;
using namespace utils;

Block::Block(Image* image) : moving_(false) {
    Initialize(image);
    set_hotspot(Vector2D(Constants::WALL_HOTSPOT_X, Constants::WALL_HOTSPOT_Y * 0.7f));
    Vector2D new_size(size().x, size().y * 0.7f);
    set_size(new_size);

    collision_type_ = STATIC;
    bound_ = new RectObject(1.0f, 1.0f);
}
Block::~Block() {}

void Block::Update(float delta_t) {
    WorldObject::Update(delta_t);
    if(moving_) {
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
        if(moving_time_left_ <= 0.0f) moving_ = false;
    }
}

void Block::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

void Block::CollidesWith(Projectile * obj) {
    if(moving_) return; // One cannot affect a block while it moves.
    Vector2D pushdir = (obj->world_position() - this->world_position()).Normalize();

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

}
