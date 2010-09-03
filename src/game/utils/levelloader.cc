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
#include<iostream>
namespace utils {

using namespace std;
using namespace scene;
using namespace sprite;
using namespace framework;

#define WALL         'W'
#define DOOR         'D'
#define MUMMY        'M'
#define RANGED_MUMMY 'R'
#define BIG_MUMMY    'B'
#define PHARAOH      'P'
#define HERO         'H'
#define FLOOR        'X'
#define EMPTY        'O'
#define POTIONL      'L'
#define POTIONM		 'N'	
#define POTIONS		 'S'	

void LevelLoader::LoadMatrix(string file_name) {
    FILE* file = fopen(file_name.c_str(),"r");
    int width, height;
    fscanf(file,"%d", &width);
    fscanf(file,"%d", &height);
    char **matrix = new char*[height];

    for (int i = 0; i < height; i++) matrix[i] = new char[width];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            fscanf(file,"\n %c \n", &matrix[i][j]);
        }
    }
    world_->set_level_width(width);
    world_->set_level_height(height);
    world_->set_level_matrix(matrix);
    fclose(file);
}
/*
 * Le o arquivo de texto e passa as informa��es de que objetos criar, e onde,
 * ao World. O arquivo de entrada deve come�ar com dois inteiros que representam
 * o tamanho do mapa seguida da matriz de simbolos associados aos tipos de
 * objeto a serem criados:
 */
bool isWall(World* world_, int i, int j) {
    char **matrix = world_->level_matrix();
    int width = world_->level_width(),
        height = world_->level_height();
    if(i >= height || j >= width || i < 0 || j < 0)
        return false;
    return matrix[i][j] == WALL;
}

void LevelLoader::Load(string file_name) {

    char    token;
    bool    hero_created = false;

    Vector2D        position;

    LoadMatrix(file_name);
    char **matrix = world_->level_matrix();
    int width = world_->level_width(),
        height = world_->level_height();

    Wall *wall_matrix[height][width];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            token = matrix[i][j];
            position.x = static_cast<float>(j);
            position.y = static_cast<float>(height - i - 1);
            if (token != EMPTY) {
                switch(token) {
                    case WALL: {
                        wall_matrix[i][j] = world_->AddWall(position);
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
                    case BIG_MUMMY: {
                        world_->AddMummy(position, 1);
                        world_->AddFloor(position);
                        break;
                    }
                    case RANGED_MUMMY: {
                        world_->AddMummy(position, 2);
                        world_->AddFloor(position);
                        break;
                    }
					case PHARAOH: {
                        world_->AddPharaoh(position);
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
                    case POTIONL: {
                        world_->AddPotion(position, 0);
                        world_->AddFloor(position);
                        break;
                    }
                    case POTIONM: {
                    	world_->AddPotion(position, 1);
                        world_->AddFloor(position);
                        break;
                    }
                    case POTIONS: {
                    	world_->AddPotion(position, 2);
                        world_->AddFloor(position);
                        break;
                    }
                }
            }
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(matrix[i][j] == WALL) {
                if(isWall(world_, i, j-1)) {
                    if(isWall(world_, i+1, j))
                        wall_matrix[i][j]->set_type(Wall::MIDDLE);
                    else
                        wall_matrix[i][j]->set_type(Wall::RIGHT);
                } else {
                    if(isWall(world_, i+1, j))
                        wall_matrix[i][j]->set_type(Wall::BOTTOM);
                    else
                        wall_matrix[i][j]->set_type(Wall::BOTTOMRIGHT);
                }
            }
        }
    }
}

} // namespace utils
