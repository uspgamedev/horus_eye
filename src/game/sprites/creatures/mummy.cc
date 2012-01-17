#include <cmath>
#include <sstream>
#include <ugdk/base/engine.h>
#include <ugdk/action/animation.h>
#include <ugdk/audio/audiomanager.h>

#include <pyramidworks/geometry/circle.h>

#include "mummy.h"

#include "game/scenes/world.h"
#include "game/utils/geometryprimitives.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"
#include "game/utils/imagefactory.h"
#include "game/utils/tile.h"
#include "game/sprites/item.h"
#include "game/builders/itembuilder.h"
#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"

namespace sprite {

using namespace ugdk;
using scene::World;
using namespace utils;


COLLISION_DIRECT(Mummy*, MummyAntiStackCollision, voiddata) {
    WorldObject *obj = (WorldObject *) voiddata; 
    data_->MummyAntiStack(obj);
}

Mummy::Mummy(Image* img) {
    Initialize(img, ANIMATIONS);

    // Animations
    last_standing_animation_ = standing_animations_[Animation_::DOWN];

    this->SelectAnimation(last_standing_animation_);

    standing_ = true;
	saw_hero_ = false;
    invulnerability_time_ = 300;

    identifier_ = std::string("Mummy");

    SET_COLLISIONCLASS(Mummy);
    ADD_COLLISIONLOGIC(Mummy, new MummyAntiStackCollision(this));
}

Mummy::~Mummy() {
	WORLD()->DecreaseEnemyCount();
}

void Mummy::TakeDamage(float life_points) {
    Creature::TakeDamage(life_points);
    standing_ = false;
}

void Mummy::MummyAntiStack(WorldObject *obj) {
    Vector2D deviation = (world_position() - obj->world_position()).Normalize();
    walking_direction_ = (walking_direction_ + deviation*0.9f).Normalize();
}

void Mummy::StartAttack(Creature* obj) {
    if(obj == NULL) obj = WORLD()->hero();
    float attackAngle = GetAttackingAngle(obj->position() - position());
    int attackAnimationIndex = GetAttackingAnimationIndex(attackAngle);
    waiting_animation_ = true;
    last_standing_animation_ = standing_animations_[direction_mapping_[attackAnimationIndex]];
    this->SelectAnimation(attacking_animations_[attackAnimationIndex]);
}

void Mummy::set_bound(float radius) {
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Circle(radius));
}


void Mummy::UpdateDirections(Vector2D target_pos){
	aim_destination_ = target_pos;
	Vector2D dir_animation = World::FromWorldCoordinates(target_pos) - position(); 
    float angle = GetAttackingAngle(dir_animation);
    int dir = GetAttackingAnimationIndex(angle);

    animation_direction_ = direction_mapping_[dir];

    Vector2D dir_ = target_pos - world_position(); 
    walking_direction_ = Vector2D::Normalized(dir_);
    last_standing_animation_ = (standing_animations_[animation_direction_]);
}


void Mummy::Update(float delta_t) {
    if (status_ == WorldObject::STATUS_DEAD) return;
    Creature::Update(delta_t);
    Vector2D dir(0,0);

    World *world = WORLD();
    if (world) {
        GameMap& map = world->level_matrix();
        TilePos mummy_pos = Tile::ToTilePos(world_position());
        mummy_pos.i =  map.size() - mummy_pos.i - 1;
        Tile *mummy_tile = Tile::GetFromMapPosition(map, mummy_pos);
        if (mummy_tile) {
            if(mummy_tile->visible())
                set_visible(true);
            else
                set_visible(false);
        }
    }
}

void Mummy::StartToDie() {
    Creature::StartToDie();
	int potion = rand() % 100;
	if (potion <=20){
		builder::ItemBuilder builder;
		ImageFactory* image_factory = WORLD()->image_factory();
		if(potion > 10)
			WORLD()->AddWorldObject(builder.LifePotion(image_factory->LifePotionImage()), this->last_stable_position_);
		else if(potion> 5)
			WORLD()->AddWorldObject(builder.ManaPotion(image_factory->ManaPotionImage()), this->last_stable_position_);
		else
			WORLD()->AddWorldObject(builder.SightPotion(image_factory->SightPotionImage()), this->last_stable_position_);
	}
}

void Mummy::PlayHitSound() const {
    std::stringstream ss;
    int id = 1 + (rand() % 4);

    ss << "data/samples/hit" << id << ".wav";
    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample(ss.str().c_str())->Play();
}

}
