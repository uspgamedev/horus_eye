//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/world.cc
// Implementacao da classe World.
//

#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/vector2D.h"
#include "../../framework/inputmanager.h"
#include "../sprites/worldobject.h"
#include "world.h"
#include "imagescene.h"
#include "../sprites/hero.h"
#include "../sprites/floor.h"
#include "../sprites/wall.h"
#include "../sprites/mummy.h"
#include "../sprites/door.h"
#include "../utils/hud.h"
#include <cmath>
#include <iostream>

#define SQRT_3 1.7320508075688772935274463415059

namespace scene {

using namespace framework;
using namespace sprite;

World::World() : Scene(), world_layer_(new framework::Layer()) {
    AddLayer(world_layer_);
    hero_ = new Hero;
    this->AddWorldObject(hero_);
    remaining_enemies_ = max_enemies_ = 0;
    hud_ = new utils::Hud(this);
    AddLayer(hud_);
    finished_game_ = false;
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
    	//RemoveAll();
        //Engine::reference()->quit();
        Finish();
        return;
    }

    set_visible(true);
    Scene::Update(delta_t);
    Vector2D offset;

    // Calcula a posicao da camera no mundo a partir da posicao do heroi.
    offset = Vector2D(0,0)-VIDEO_MANAGER()->video_size()*0.5;
    if(hero_) {
        offset = offset + hero_->position();
    }
    world_layer_->set_offset(offset);

    // Verifica e trata colisoes do WorldObjetcts.
    std::list<sprite::WorldObject*>::iterator i, j;

    // TODO: colisao esta sendo verificada 2x por iteracao, corrigir isso
    for (i = world_objects_.begin(); i != world_objects_.end(); ++i)
        if ((*i)->collision_type() == WorldObject::MOVEABLE)
            for (j = world_objects_.begin(); j != world_objects_.end(); ++j)
                if ( (*i) != (*j) && (*j)->collision_type() != WorldObject::NO_COLLISION &&
                        (*i)->IsColliding(*j)) {
                    (*i)->HandleCollision(*j);
                    (*j)->HandleCollision(*i);
                }

    RemoveInactiveObjects();
    if (!hero_) FinishLevel(false);
    if (finished_game_) {
        Finish();
    }

}

void World::End() {
    this->RemoveAll();
    ImageScene *ending;
    if (good_end_)
        ending = new ImageScene(NULL, NULL, 5);
    else
        ending = new ImageScene(NULL, NULL, 5);
    Engine::reference()->PushScene(ending);
    set_visible(false);

}

// Nao nos importamos com a ordem dos WorldObjects nas listas;
// Decidimos fazer as adicoes e remocoes no comeco das listas por padrao.
void World::AddWorldObject(sprite::WorldObject* new_object) {
    if(new_object->collision_type() == WorldObject::NO_COLLISION)
        collisionless_objects.push_front(new_object);
    else
        world_objects_.push_front(new_object);
    world_layer_->AddSprite(new_object);
}

void World::AddFloor(framework::Vector2D &pos) {
	Floor *floor = new Floor;
	floor->set_world_position(pos);
	this->AddWorldObject(floor);
}

void World::AddWall(framework::Vector2D &pos) {
	Wall *wall = new Wall;
	wall->set_world_position(pos);
	this->AddWorldObject(wall);
}

void World::AddMummy(framework::Vector2D &pos) {
	Mummy *mummy = new Mummy;
	mummy->set_world_position(pos);
	this->AddWorldObject(mummy);
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

void World::FinishLevel(bool goodEnd) {
    good_end_ = goodEnd;
    finished_game_ = true;
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
		}
	world_objects_.remove_if(worldObjectIsDead);
}

void World::RemoveAll() {

    std::list<sprite::WorldObject*>::iterator i;
    for (i = world_objects_.begin(); i != world_objects_.end(); ++i) {
		world_layer_->RemoveSprite(*i);
		delete (*i);
    }
    world_objects_.clear();
    for (i = collisionless_objects.begin(); i != collisionless_objects.end(); ++i) {
		world_layer_->RemoveSprite(*i);
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

} // namespace scene
