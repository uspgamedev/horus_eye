#include <cmath>
#include <sstream>
#include <ugdk/base/engine.h>
#include <ugdk/action/animation.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/time/timeaccumulator.h>

#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "mummy.h"

#include "game/components/mummycontroller.h"
#include "game/components/animation.h"
#include "game/scenes/world.h"
#include "game/utils/geometryprimitives.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"
#include "game/utils/imagefactory.h"
#include "game/utils/tile.h"
#include "game/builders/itembuilder.h"
#include "game/skills/combatart.h"

namespace component {

using namespace ugdk;
using scene::World;
using namespace utils;

COLLISION_DIRECT(Mummy*, MummyAntiStackCollision, voiddata) {
    sprite::WorldObject *obj = (sprite::WorldObject *) voiddata; 
    data_->MummyAntiStack(obj);
}

Mummy::Mummy(sprite::WorldObject* owner, double speed) 
    :   Creature(owner, speed) {

    Creature::AddKnownCollisions();
    owner_->collision_object()->AddCollisionLogic("Mummy", new MummyAntiStackCollision(this));
}

Mummy::~Mummy() {
    WORLD()->DecreaseEnemyCount();
}

void Mummy::MummyAntiStack(sprite::WorldObject *obj) {
    Vector2D deviation = (owner_->world_position() - obj->world_position()).Normalize();
    walking_direction_ = (walking_direction_ + deviation*0.9).Normalize();
}

void Mummy::OnWorldAdd(scene::World* world) {
    world->IncreaseNumberOfEnemies();
}

void Mummy::Update(double delta_t) {
    Creature::Update(delta_t);
    World *world = WORLD();
    if (world) {
        GameMap& map = world->level_matrix();
        TilePos mummy_pos = Tile::ToTilePos(owner_->world_position());
        mummy_pos.i =  map.size() - mummy_pos.i - 1;
        Tile *mummy_tile = Tile::GetFromMapPosition(map, mummy_pos);
        if (mummy_tile) {
            if(mummy_tile->visible())
                owner_->node()->modifier()->set_visible(true);
            else
                owner_->node()->modifier()->set_visible(false);
        }
    }
    if(owner_->is_active()) {
        component::Controller* controller = owner_->controller();
        if(!owner_->animation()->is_uninterrutible()) {
            UseSkills();
        }
        if(!owner_->animation()->is_uninterrutible()) {
            walking_direction_ = controller->direction_vector();
            const Direction& direction = controller->direction();
            if(direction) {
                last_standing_direction_ = direction;
                owner_->animation()->set_animation(utils::WALKING);
                owner_->animation()->set_direction(direction);
            } else {
                owner_->animation()->set_animation(utils::STANDING);
                owner_->animation()->set_direction(last_standing_direction_);
            }
            Creature::Move(this->GetWalkingDirection(), delta_t);
        }
    }
    speed_ = original_speed_;
}

}
