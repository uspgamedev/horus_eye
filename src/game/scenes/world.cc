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
#include "pausescene.h"
#include "../sprites/worldobject.h"
#include "../sprites/hero.h"
#include "../utils/hud.h"
#include "../utils/fog.h"
#include "../utils/levelmanager.h"
namespace scene {

using namespace framework;
using namespace sprite;
using namespace utils;

World::World(sprite::Hero *hero) : Scene(), world_layer_(new framework::Layer()) {
    AddLayer(world_layer_);

	image_factory_ = new utils::ImageFactory();

    fog_ = new Fog();
    AddLayer(fog_);

    hero_ = hero;

    hud_ = new utils::Hud(this);
    AddLayer(hud_);

    remaining_enemies_ = max_enemies_ = 0;
    level_state_ = LevelManager::NOT_FINISHED;
    player_exit_ = false;
}

// Destrutor
World::~World() {
}

bool worldObjectIsDead (const WorldObject* value) {
	bool is_dead = ((*value).status() == WorldObject::STATUS_DEAD);
	if (is_dead) delete value;
    return is_dead;
}

void World::HandleCollisions() {
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
}

void World::VerifyCheats() {
    InputManager *input = Engine::reference()->input_manager();

	if (input->KeyPressed(K_p)) {
	    LevelManager *level_manager = LevelManager::reference();
	    level_manager->SetNextLevel(level_manager->GetNextLevelID() + 1);
	    level_state_ = LevelManager::FINISH_WARP;
	} else if (input->KeyPressed(K_o)) {
	    LevelManager *level_manager = LevelManager::reference();
	    unsigned int cur_level = level_manager->GetNextLevelID();
	    if(cur_level > 0) {
	        level_manager->SetNextLevel(cur_level - 1);
	        level_state_ = LevelManager::FINISH_WARP;
	    }
	}
	if(input->KeyPressed(K_h)) {
		hero_->set_life(hero_->max_life());
		hero_->set_mana(hero_->max_mana());
	}
}

Vector2D World::ActualOffset() {
	Vector2D result = Vector2D(0,0)-VIDEO_MANAGER()->video_size()*0.5;
	if(hero_) result = result + hero_->position();

	return result;
}

bool World::VerifyQuit() {
    InputManager *input = Engine::reference()->input_manager();
    if(input->KeyDown(K_ESCAPE)) {
        LevelManager::reference()->FinishLevel(LevelManager::FINISH_QUIT);
        return true;
    }
	return false;
}

bool World::VerifyPause() {
    InputManager *input = Engine::reference()->input_manager();
    if(input->KeyPressed(K_RETURN)) {
        Engine::reference()->PushScene(new PauseScene);
        return true;
    }
    return false;
}

void World::Update(float delta_t) {

	if(VerifyQuit()) return;
	if(VerifyPause()) return;

    set_visible(true);
    Scene::Update(delta_t);

    world_layer_->set_offset(ActualOffset());
    fog_->set_offset(ActualOffset());

	HandleCollisions();

    RemoveInactiveObjects();
    AddNewWorldObjects();
    
	if (!hero_)
        level_state_ = LevelManager::FINISH_DIE;

	VerifyCheats();

    if (level_state_ != LevelManager::NOT_FINISHED)
        LevelManager::reference()->FinishLevel(level_state_);
}

void World::End() {
    this->RemoveAll();
}

void World::IncreaseNumberOfEnemies() {
	remaining_enemies_++;
	max_enemies_++;
}

void World::AddWorldObject(sprite::WorldObject* new_object, framework::Vector2D pos) {

	new_object-> set_world_position(pos);
    new_world_objects.push_front(new_object);
}

void World::AddNewWorldObjects() {
    for (list<sprite::WorldObject*>::iterator it = new_world_objects.begin();
         it != new_world_objects.end();
         ++it) {

        WorldObject *new_object = *it;
        if(new_object->collision_type() == WorldObject::NO_COLLISION) {
            collisionless_objects.push_front(new_object);
        } else {
            world_objects_.push_front(new_object);
        }

        world_layer_->AddSprite(new_object);
        fog_->AddLightSource(new_object);
    }
    new_world_objects.clear();
}

void World::AddHero(framework::Vector2D pos) {
    this->AddWorldObject(hero_, pos);
}

int World::CountRemainingEnemies() {
    return 	remaining_enemies_;
}

void World::RemoveInactiveObjects() {
    std::list<sprite::WorldObject*>::iterator i;
    if(hero_ != NULL && hero_->status() == WorldObject::STATUS_DEAD) {
        hero_ = NULL;
    }
	for (i = world_objects_.begin(); i != world_objects_.end(); ++i) {
		if((*i)->status() == WorldObject::STATUS_DEAD) {
			world_layer_->RemoveSprite(*i);
			fog_->RemoveLightSource(*i);
		}
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
    Vector2D tx(1/sqrt(3.0), -1/sqrt(3.0));
    Vector2D ty(-1, -1);
    return (tx * screen_coords.x)   +  (ty * screen_coords.y);
}

Vector2D World::FromWorldLinearCoordinates(Vector2D world_coords) {
    Vector2D tx(sqrt(3.0)/2, -.5);
    Vector2D ty(-sqrt(3.0)/2, -.5);
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
