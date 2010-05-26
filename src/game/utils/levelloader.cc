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

/*
 * TODO: nao terminada!
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

    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++) {
            fscanf(level_input, "%c", &token);
            position.x = i;
            position.y = j;
            // ... Criar o chao.
            world_->AddFloor(new_floor);
            if (token != 'X') {
                // ... Criar o objeto correspondente.
                world_->AddWorldObject(new_world_obj);
            }
        }


}

}
