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
#include <cmath>
#include <iostream>

#define SQRT_3 1.7320508075688772935274463415059

namespace scene {

using namespace framework;
using namespace sprite;

// Destrutor
World::~World() {
}

bool worldObjectIsDead (const WorldObject& value) {
    return (value.status() == WorldObject::STATUS_DEAD);
}

void World::Update(float delta_t) {

    Scene::Update(delta_t);
    Vector2D offset;

    // Calcula a posicao da camera no mundo a partir da posicao do heroi.
    offset = Vector2D(-512, -384);
    if(hero_) {
        offset = offset + hero_->position();
    }
    world_layer_->set_offset(offset);

    // Verifica e trata colisoes do WorldObjetcts.
    std::list<sprite::WorldObject*>::iterator i, j;

    for (i = world_objects_.begin(); i != world_objects_.end(); ++i)
        if ((*i)->collision_type() == WorldObject::MOVEABLE)
            for (j = world_objects_.begin(); j != world_objects_.end(); ++j)
                if ( (*i) != (*j) && (*j)->collision_type() != WorldObject::NO_COLLISION &&
                        (*i)->IsColliding(*j)) {
                    (*i)->HandleCollision(*j);
                    (*j)->HandleCollision(*i);
                }

    for (i = world_objects_.begin(); i != world_objects_.end(); ++i)
        if((*i)->status() == WorldObject::STATUS_DEAD)
            world_layer_->RemoveSprite(*i);

    world_objects_.remove_if(worldObjectIsDead);
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
    return (transformed * 40);
}

} // namespace scene
