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
    
    scanf("%d", &width_);
    scanf("%d", &height_);
    matrix_ = new char*[height_];
    for (int i = 0; i < height_; i++) matrix_[i] = new char[width_];

    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            scanf("\n %c \n", &matrix_[i][j]);
        }
    }
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

    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            token = matrix_[i][j];
            position.x = j;
            position.y = height_ - i - 1;
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
                    case FLOOR: {
                        world_->AddFloor(position);
                        break;
                    }
                    case DOOR: {
                        world_->AddDoor(position);
                    }
                }
            }
        }
    }
    world_->set_level_width(width_);
    world_->set_level_height(height_);
    world_->set_level_matrix(matrix_);
}

} // namespace utils
