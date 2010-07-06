//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/levelloader.cc
// Definicao da classe LevelLoader.
//

#include "levelloader.h"
#include <cstdio>
#include "../sprites/worldobject.h"
#include "../sprites/hero.h"
#include "../sprites/mummy.h"
#include "../sprites/floor.h"
#include "../sprites/wall.h"

namespace utils {

using namespace std;
using namespace scene;
using namespace sprite;
using namespace framework;

#define WALL    'W'
#define DOOR    'D'
#define MUMMY   'M'
#define HERO    'H'
#define EMPTY   'X'

/*
 * Le o arquivo de texto e passa as informa��es de que objetos criar, e onde,
 * ao World. O arquivo de entrada deve come�ar com dois inteiros que representam
 * o tamanho do mapa seguida da matriz de simbolos associados aos tipos de
 * objeto a serem criados:
 * Wall     : W
 * Door     : D (nao implementado)
 * Mummy    : M 
 * Hero     : H
 * Empty    : X
 */
void LevelLoader::Load(string file_name) {

    FILE *  level_input;
    char    token;
    int     width, height;
    bool    hero_created = false;

    level_input = fopen(file_name.c_str(), "r");

    fscanf(level_input, "%d", &width);
    fscanf(level_input, "%d", &height);

    Floor *         new_floor;
    WorldObject *   new_world_obj = NULL;
    Vector2D        position;

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            fscanf(level_input, "\n %c \n", &token);
            position.x = i;
            position.y = j;
            new_floor = new Floor;
            new_floor->set_world_position(position);
            world_->AddFloor(new_floor);
            if (token != EMPTY) {
                switch(token) {
                    case WALL: {
                        new_world_obj = new Wall();
                        break;
                    }
                    case HERO: {
                        if (!hero_created) {
                            Hero *hero = new Hero();
                            new_world_obj = hero;
                            world_->set_hero(hero);
                            world_->AddMoveable(hero);
                            hero_created = true;
                        }
                        break;
                    }
                    case MUMMY: {
                        new_world_obj = new Mummy();
                        break;
                    }

                    default: {
                        new_world_obj = NULL;
                        break;
                    }
                }
                if(new_world_obj != NULL) {
                    new_world_obj->set_world_position(position);
                    world_->AddWorldObject(new_world_obj);
                    new_world_obj = NULL;
                }
            }
        }
    }


}

} // namespace utils
