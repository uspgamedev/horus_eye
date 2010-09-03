//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/world.cc
// Implementacao da classe World.
//

#include <cmath>
#include <iostream>
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/scene.h"
#include "../../framework/vector2D.h"
#include "../../framework/inputmanager.h"
#include "../../framework/fogmanager.h"
#include "world.h"
#include "imagescene.h"
#include "../sprites/worldobject.h"
#include "../sprites/hero.h"
#include "../sprites/floor.h"
#include "../sprites/wall.h"
#include "../sprites/mummy.h"
#include "../sprites/pharaoh.h"
#include "../sprites/door.h"
#include "../sprites/item.h"
#include "../sprites/weapons/mummyweapon.h"
#include "../sprites/weapons/mummyrangedweapon.h"
#include "../sprites/weapons/pharaohrangedweapon.h"
#include "../utils/hud.h"
#include "../utils/fog.h"
#include "../utils/levelmanager.h"

#define SQRT_3 1.7320508075688772935274463415059f

namespace scene {

using namespace framework;
using namespace sprite;
using namespace utils;

World::World(sprite::Hero *hero) : Scene(), world_layer_(new framework::Layer()) {
    AddLayer(world_layer_);

    fog_ = new Fog();
    AddLayer(fog_);

    hero_ = hero;
    this->AddWorldObject(hero_);

    hud_ = new utils::Hud(this);
    AddLayer(hud_);

    remaining_enemies_ = max_enemies_ = 0;
    level_state_ = LevelManager::NOT_FINISHED;
    player_exit_ = false;
}

// Destrutor
World::~World() {
    for (int i = 0; i < level_height_; ++i)
        delete[] level_matrix_[i];
    delete[] level_matrix_;
}

bool worldObjectIsDead (const WorldObject* value) {
	bool is_dead = ((*value).status() == WorldObject::STATUS_DEAD);
	if (is_dead) delete value;
    return is_dead;
}

void World::set_level_width(int width) {
    level_width_ = width;
}

void World::set_level_height(int height) {
    level_height_ = height;
}

void World::set_level_matrix(char ** matrix) {
    level_matrix_ = matrix;
}

int World::level_width() {
    return level_width_;
}

int World::level_height() {
    return level_height_;
}

char** World::level_matrix() {
    return level_matrix_;
}

void World::Update(float delta_t) {

    InputManager *input_ = Engine::reference()->input_manager();
    if(input_->KeyDown(K_ESCAPE)) {
        LevelManager::reference()->FinishLevel(LevelManager::FINISH_QUIT);
    	/*player_exit_ = true;
        Finish();*/
        return;
    }

    set_visible(true);
    Scene::Update(delta_t);
    Vector2D offset;

    // Calcula a posicao da camera no mundo a partir da posicao do heroi.
    offset = Vector2D(0,0)-VIDEO_MANAGER()->video_size()*0.5;

    if(hero_)
        offset = offset + hero_->position();

    world_layer_->set_offset(offset);
    fog_->set_offset(offset);

    // Verifica e trata colisoes do WorldObjetcts.
    std::list<sprite::WorldObject*>::iterator i, j;

    // TODO: colisao esta sendo verificada 2x por iteracao, corrigir isso
    for (i = world_objects_.begin(); i != world_objects_.end(); ++i) {
        if ((*i)->collision_type() == WorldObject::MOVEABLE) {
            for (j = world_objects_.begin(); j != world_objects_.end(); ++j) {
                if ( (*i) != (*j) && (*j)->collision_type() != WorldObject::NO_COLLISION &&
                        (*i)->IsColliding(*j)) {
                    (*i)->HandleCollision(*j);
                    (*j)->HandleCollision(*i);
                }
			}
		}
	}

    RemoveInactiveObjects();
    
	if (!hero_)
        level_state_ = LevelManager::FINISH_DIE;

	if(input_->KeyPressed(K_p)) {
	    LevelManager *level_manager = LevelManager::reference();
	    level_manager->SetNextLevel(level_manager->GetNextLevelID() + 1);
	    level_state_ = LevelManager::FINISH_WARP;
	} else if(input_->KeyPressed(K_o)) {
	    LevelManager *level_manager = LevelManager::reference();
	    unsigned int cur_level = level_manager->GetNextLevelID();
	    if(cur_level > 0) {
	        level_manager->SetNextLevel(cur_level - 1);
	        level_state_ = LevelManager::FINISH_WARP;
	    }
	}

    if (level_state_ != LevelManager::NOT_FINISHED)
        LevelManager::reference()->FinishLevel(level_state_);
}

void World::End() {
    this->RemoveAll();
}

// Nao nos importamos com a ordem dos WorldObjects nas listas;
// Decidimos fazer as adicoes e remocoes no comeco das listas por padrao.
void World::AddWorldObject(sprite::WorldObject* new_object) {
    if(new_object->collision_type() == WorldObject::NO_COLLISION)
        collisionless_objects.push_front(new_object);
    else
        world_objects_.push_front(new_object);
    world_layer_->AddSprite(new_object);
    fog_->AddLightSource(new_object);
}

void World::AddFloor(framework::Vector2D &pos) {
	Floor *floor = new Floor;
	floor->set_world_position(pos);
	this->AddWorldObject(floor);
}

Wall* World::AddWall(framework::Vector2D &pos) {
	Wall *wall = new Wall;
	wall->set_world_position(pos);
	this->AddWorldObject(wall);
	return wall;
}

void World::AddMummy(framework::Vector2D &pos, int type) {
	Mummy *mummy = NULL;

    switch (type) {
        case 0:
            mummy = mummy_builder_.standard_mummy();
            break;

        case 1:
            mummy = mummy_builder_.big_mummy();
            break;

        case 2:
            mummy = mummy_builder_.ranged_mummy();
            break;
    }
            
	mummy->set_world_position(pos);
	this->AddWorldObject(mummy);
	remaining_enemies_++;
	max_enemies_++;
}

void World::AddPharaoh(framework::Vector2D &pos) {
	Pharaoh *pharaoh = mummy_builder_.pharaoh();

	pharaoh->set_world_position(pos);
	this->AddWorldObject(pharaoh);
	remaining_enemies_++;
	max_enemies_++;
}

void World::AddHero(framework::Vector2D &pos) {
	hero_->set_world_position(pos);
}

void World::AddDoor(framework::Vector2D &pos) {

    Door *door = new Door;
    door->set_world_position(pos);
    this->AddWorldObject(door);

}

void World::AddPotion(framework::Vector2D &pos, int type) {
    Item *potion = NULL;

    switch (type) {
        case 0:
            potion = potion_builder_.life_potion();
            break;

        case 1:
            potion = potion_builder_.mana_potion();
            break;

        case 2:
        	potion = potion_builder_.sight_potion();
        	break;
    }
    
    potion->set_world_position(pos);
    this->AddWorldObject(potion);
    
}

int World::CountRemainingEnemies() {
    return 	remaining_enemies_;
}

void World::RemoveInactiveObjects() {
    std::list<sprite::WorldObject*>::iterator i, j;
    if(hero_ != NULL && hero_->status() == WorldObject::STATUS_DEAD)
        hero_ = NULL;
	for (i = world_objects_.begin(); i != world_objects_.end(); ++i)
		if((*i)->status() == WorldObject::STATUS_DEAD) {
			world_layer_->RemoveSprite(*i);
			fog_->RemoveLightSource(*i);
		}
	world_objects_.remove_if(worldObjectIsDead);
}

void World::RemoveAll() {

    std::list<sprite::WorldObject*>::iterator i;
    for (i = world_objects_.begin(); i != world_objects_.end(); ++i) {
		world_layer_->RemoveSprite(*i);
		fog_->RemoveLightSource(*i);
		if ( *i != hero_ ) {
			delete (*i);
		}
    }
    world_objects_.clear();
    for (i = collisionless_objects.begin(); i != collisionless_objects.end(); ++i) {
		world_layer_->RemoveSprite(*i);
		fog_->RemoveLightSource(*i);
		delete (*i);
    }
    collisionless_objects.clear();
    hero_ = NULL;

}


Vector2D World::FromScreenLinearCoordinates(Vector2D screen_coords) {
    Vector2D tx(1/SQRT_3, -1/SQRT_3);
    Vector2D ty(-1, -1);
    return (tx * screen_coords.x)   +  (ty * screen_coords.y);
}

Vector2D World::FromWorldLinearCoordinates(Vector2D world_coords) {
    Vector2D tx(SQRT_3/2, -.5);
    Vector2D ty(-SQRT_3/2, -.5);
    return (tx * world_coords.x)   +  (ty * world_coords.y);
}

Vector2D World::FromWorldCoordinates(Vector2D world_coords) {
    Vector2D transformed = FromWorldLinearCoordinates(world_coords);
    return (transformed * 60);
}

Vector2D World::FromScreenCoordinates(Vector2D screen_coords) {
    Vector2D    global_screen_coords = screen_coords + WORLD()->world_layer_->offset(),
                transformed = FromScreenLinearCoordinates(global_screen_coords);
    return (transformed * (1.0f/60.0f));
}

Image* World::CreateFogTransparency(float radius) {
    Vector2D ellipse_coords = Vector2D(2, 1) * radius * 60;
    return Engine::reference()->fog_manager()->GetLightSource(ellipse_coords);
}

} // namespace scene
