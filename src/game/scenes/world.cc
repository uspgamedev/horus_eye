//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/world.cc
// Implementacao da classe World.
//

#include "../../framework/scene.h"
#include "../../framework/vector2D.h"
#include "../sprites/worldobject.h"
#include "world.h"
#include "../sprites/hero.h"
#include "../sprites/floor.h"
#include "../sprites/wall.h"

#define SQRT_3 1.7320508075688772935274463415059

namespace scene {

using namespace framework;
using namespace sprite;

// Destrutor
World::~World() {
}

void World::Update(float delta_t) {

    Scene::Update(delta_t);

    if(hero_) {
        // Calcula a posicao da camera no mundo a partir da posicao do heroi.
        float   x = hero_->world_position().x,
                y = hero_->world_position().y;

        camera_position_.x = -512 + SQRT_3*(x-y)/2.0;
        camera_position_.y = -384 - (x+y)/2.0;
    }
    else {
        camera_position_.x = -512;
        camera_position_.y = -384;
    }

    // Verifica e trata colisoes do WorldObjetcts.
    std::list<sprite::WorldObject*>::iterator i, j;

    for (i = collidable_.begin(); i != collidable_.end(); ++i)
        for (j = i, ++j; j != collidable_.end(); ++j)
            if ((*i)->IsColliding(*j)) {
                (*i)->HandleCollision(*j);
                (*j)->HandleCollision(*i);
            }
}

// Nao nos importamos com a ordem dos WorldObjects nas listas;
// Decidimos fazer as adicoes e remocoes no comeco das listas por padrao.
void World::AddWorldObject(sprite::WorldObject* new_object) {
    world_objects_.push_front(new_object);
    collidable_.push_front(new_object);
    world_layer_->AddSprite(new_object);
}

void World::AddFloor(sprite::Floor* new_floor) {
    world_objects_.push_front(new_floor);
    world_layer_->AddSprite(new_floor);
}

void World::FinishLevel(bool goodEnd) {
}

int World::CountRemainingEnemies() {
    return 0; // Aguardando implementacao da classe Mumia
}


// get Hero
Hero * World::hero() {
    return hero_;
}

void World::RemoveInactiveObjects() {
}

} // namespace scene
