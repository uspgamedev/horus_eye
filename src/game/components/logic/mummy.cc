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

#define EXP_PARAM (1.0)

// Devolve um tempo ~exp(EXP_PARAM)
static int WaitingTime () {
    return (int)(1000*-log(1.0*rand()/RAND_MAX)/EXP_PARAM);
}

COLLISION_DIRECT(Mummy*, MummyAntiStackCollision, voiddata) {
    sprite::WorldObject *obj = (sprite::WorldObject *) voiddata; 
    data_->MummyAntiStack(obj);
}

static void MummyDeath(sprite::WorldObject* wobj) {
    int potion = rand() % 100;
    if (potion <=20){
        std::vector<std::string> blank;
        if(potion > 10)
            WORLD()->AddWorldObject(builder::ItemBuilder::LifePotion(blank), wobj->world_position());
        else if(potion> 5)
            WORLD()->AddWorldObject(builder::ItemBuilder::ManaPotion(blank), wobj->world_position());
        else
            WORLD()->AddWorldObject(builder::ItemBuilder::SightPotion(blank), wobj->world_position());
    }
}

Mummy::Mummy(sprite::WorldObject* owner) 
    :   Creature(owner, new MummyController(owner)) {

    // Animations
	owner->set_caster(new Caster(owner));
    owner->animation()->set_animation(utils::STANDING);
    owner->animation()->set_direction(last_standing_direction_);
    time_to_think_ = TIME_TO_THINK;
    standing_ = true;
    interval_ = new ugdk::time::TimeAccumulator(0);
    //TODO
    //invulnerability_time_ = 300;
    //identifier_ = std::string("Mummy");
    owner_->collision_object()->InitializeCollisionClass("Mummy");
    Creature::AddKnownCollisions();
    owner_->collision_object()->AddCollisionLogic("Mummy", new MummyAntiStackCollision(this));

    owner_->set_start_to_die_callback(MummyDeath);
}

Mummy::~Mummy() {
    if (interval_) delete interval_;
    WORLD()->DecreaseEnemyCount();
}

void Mummy::MummyAntiStack(sprite::WorldObject *obj) {
    Vector2D deviation = (owner_->world_position() - obj->world_position()).Normalize();
    walking_direction_ = (walking_direction_ + deviation*0.9).Normalize();
}

void Mummy::StartAttack(sprite::WorldObject* obj) {
    if(obj == NULL) obj = WORLD()->hero_world_object();
    if(obj == NULL) return;
    Direction d = Direction::FromScreenVector(obj->node()->modifier()->offset() - owner_->node()->modifier()->offset());
    last_standing_direction_ = d;
    owner_->animation()->set_direction(d);
    owner_->animation()->set_animation(utils::ATTACKING);
    owner_->animation()->flag_uninterrutible();
}

void Mummy::set_bound(double radius) {
    owner_->collision_object()->set_shape(new pyramidworks::geometry::Circle(radius));
}

void Mummy::OnWorldAdd(scene::World* world) {
    world->IncreaseNumberOfEnemies();
}

void Mummy::RandomMovement(){
    double PI = acos(-1.0);

    if (interval_->Expired()) {

        int dir = rand() % 8;

        Direction d;
        if (dir < 3) d |= Direction::Up();
        if (dir >= 2 && dir < 5) d |= Direction::Left();
        if (dir >= 4 && dir < 7) d |= Direction::Down();
        if (dir >= 6 || dir == 0) d |= Direction::Right();
        animation_direction_ = d;

        interval_->Restart(WaitingTime());
        last_direction_ = walking_direction_ = Vector2D(cos(dir*PI/4.0),sin(dir*PI/4.0));
    }
}

void Mummy::UpdateDirection(Vector2D destination){
    Vector2D dir_animation = World::FromWorldCoordinates(destination) - owner_->node()->modifier()->offset(); 
    Direction d = Direction::FromScreenVector(dir_animation);
    animation_direction_ = d;

    Vector2D dir_ = path_.front() - owner_->world_position();
    last_direction_ = walking_direction_ = Vector2D::Normalized(dir_);
    last_standing_direction_ = animation_direction_;
}

void Mummy::Think(double dt) {
    time_to_think_ -= dt;
    if(time_to_think_ <= 0){
        time_to_think_ = TIME_TO_THINK;
        speed_ = original_speed_;
        VisionStrategy strategy;
        if(strategy.IsVisible(owner_->world_position())){
            standing_ = false;
            
            path_ = strategy.Calculate(owner_->world_position());
            if(!path_.empty()) UpdateDirection(path_.front());
            
            if(weapon_->Available()) {
                //if(!path_.empty()) aim_destination_ = path_.front();
                if(weapon_->IsValidUse()){
                    weapon_->Use();
                    this->StartAttack(NULL);
                    speed_ = 0;
                }
            }
        }
        else if(!standing_){
            RandomMovement();
            last_standing_direction_ = animation_direction_;
        }
    }
}

void Mummy::Update(double delta_t) {
    Creature::Update(delta_t);
    Vector2D dir(0,0);

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

    if (!owner_->animation()->is_uninterrutible() && owner_->is_active()) {
        Think(delta_t);

        if(!owner_->animation()->is_uninterrutible()) {
            Creature::Move(this->GetWalkingDirection(), delta_t);
            walking_direction_ = last_direction_;
            owner_->animation()->set_animation(utils::WALKING);
            owner_->animation()->set_direction(animation_direction_);
        }
    }

}

}
