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
#define FLOOR   'X'
#define EMPTY   'O'

void LevelLoader::LoadMatrix(string file_name) {

    freopen(file_name.c_str(),"r",stdin);
    
    int width, height;
    scanf("%d", &width);
    scanf("%d", &height);
    char **matrix = new char*[height];
    for (int i = 0; i < height; i++) matrix[i] = new char[width];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            scanf("\n %c \n", &matrix[i][j]);
        }
    }
    world_->set_level_width(width);
    world_->set_level_height(height);
    world_->set_level_matrix(matrix);


}
/*
 * Le o arquivo de texto e passa as informa��es de que objetos criar, e onde,
 * ao World. O arquivo de entrada deve come�ar com dois inteiros que representam
 * o tamanho do mapa seguida da matriz de simbolos associados aos tipos de
 * objeto a serem criados:
 * Wall     : W
 * Door     : D (nao implementado)
 * Mummy    : M 
 * Hero     : H
 * Floor    : X
 * Empty    : O
 */
void LevelLoader::Load(string file_name) {

    char    token;
    bool    hero_created = false;

    Vector2D        position;

    LoadMatrix(file_name);
    char **matrix = world_->level_matrix();
    int width = world_->level_width(),
        height = world_->level_height();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            token = matrix[i][j];
            position.x = j;
            position.y = height - i - 1;
            if (token != EMPTY) {
                switch(token) {
                    case WALL: {
                        world_->AddWall(position);
                        break;
                    }
                    case HERO: {
                        if (!hero_created) {
                            world_->AddHero(position);
                            world_->AddFloor(position);
                            hero_created = true;
                        }
                        break;
                    }
                    case MUMMY: {
                        world_->AddMummy(position);
                        world_->AddFloor(position);
                        break;
                    }
                    case DOOR: {
                        if (j < width-1 && matrix[i][j+1] == DOOR) {
                            Vector2D pos = position + Vector2D(0.5,0);
                            world_->AddDoor(pos);
                        }
                        // Sem break de proposito.
                    }
                    case FLOOR: {
                        world_->AddFloor(position);
                        break;
                    }
                }
            }
        }
    }
}

} // namespace utils
