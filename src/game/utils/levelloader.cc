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

namespace utils {

using namespace std;
using namespace scene;
using namespace sprite;
using namespace framework;

#define WALL W
#define DOOR D
#define MUMMY M
#define HERO H

/*
 * Lê o arquivo de texto e passa as informações de que objetos criar, e onde, ao World.
 * O arquivo de entrada deve começar com dois inteiros que representam o tamanho do mapa
 * seguida da matriz de simbolos associados aos tipos de objeto a serem criados:
 * Wall     : W
 * Door     : D
 * Mummy    : M
 * Hero     : H
 * Empty    : X
 */
void LevelLoader::Load(string file_name) {

    FILE *  level_input;
    char    token;
    int     width, height;

    level_input = fopen(file_name.c_str(), "r");

    fscanf(level_input, "%d", &width);
    fscanf(level_input, "%d", &height);

    Floor *         new_floor;
    WorldObject *   new_world_obj;
    Vector2D        position;

    for (int j = 0; j < height; ++j)
        for (int i = 0; i < width; ++i) {
            fscanf(level_input, "%c", &token);
            position.x = i;
            position.y = j;
            new_floor = new Floor;
            new_floor->set_world_position(position);
            world_->AddFloor(new_floor);
            if (token != 'X') {
                switch(token) {
                    case WALL:
                        new_world_obj = new Wall;
                        break;
                    case HERO:
                        new_world_obj = new Hero;
                        break;
                    default:
                        break;
                }
                new_floor->set_world_position(position);
                // ... Criar o objeto correspondente.
                world_->AddWorldObject(new_world_obj);
            }
        }


}

}
