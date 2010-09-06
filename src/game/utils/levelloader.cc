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
    fscanf(file,"%d %d", &width, &height);
    
	char **matrix = new char*[height];
    for (int i = 0; i < height; i++) 
		matrix[i] = new char[width];

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

bool LevelLoader::InRange (int i,int j) {
    if(i >= world_->level_height() || j >= world_->level_width() || i < 0 || j < 0)
		return false;
	return true;
}

bool LevelLoader::IsWall(int i, int j) {
	return InRange(i,j) ? world_->level_matrix()[i][j] == WALL : false;
}

void LevelLoader::InitializeWallTypes(vector<vector<Wall *> > wall_matrix) {
    char **matrix = world_->level_matrix();
	
	for (int i = 0; i < world_->level_height(); ++i) {
		for (int j = 0; j < world_->level_width(); ++j) {
			if(matrix[i][j] == WALL) {
				if(IsWall(i, j-1)) {
					if(IsWall(i+1, j)) {
						wall_matrix[i][j]->set_type(Wall::MIDDLE);
					} else {
						wall_matrix[i][j]->set_type(Wall::RIGHT);
					}
				} else {
					if(IsWall(i+1, j)) {
						wall_matrix[i][j]->set_type(Wall::BOTTOM);
					} else {
						wall_matrix[i][j]->set_type(Wall::BOTTOMRIGHT);
					}
				}
			}
		}
	}
}

void LevelLoader::Load(string file_name) {

    LoadMatrix(file_name);
    char **matrix = world_->level_matrix();

	vector<vector<Wall* > > wall_matrix(world_->level_height(), vector<Wall *> (world_->level_width()));

    for (int i = 0; i < world_->level_height(); ++i) {
        for (int j = 0; j < world_->level_width(); ++j) {
            char token = matrix[i][j];
			Vector2D position ((float)j, (float)(world_->level_height() - i - 1));
            if (token != EMPTY) {
                switch(token) {
                    case WALL: {
                        wall_matrix[i][j] = world_->AddWall(position);
                        break;
                    }
                    case HERO: {
						world_->AddHero(position);
						world_->AddFloor(position);
                        break;
                    }
                    case MUMMY: {
                        world_->AddMummy(position);
                        world_->AddFloor(position);
                        break;
                    }
                    case BIG_MUMMY: {
                        world_->AddBigMummy(position);
                        world_->AddFloor(position);
                        break;
                    }
                    case RANGED_MUMMY: {
                        world_->AddRangedMummy(position);
                        world_->AddFloor(position);
                        break;
                    }
					case PHARAOH: {
                        world_->AddPharaoh(position);
                        world_->AddFloor(position);
                        break;
                    }
                    case DOOR: {
                        if (j < world_->level_width()-1 && matrix[i][j+1] == DOOR) {
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
                        world_->AddLifePotion(position);
                        world_->AddFloor(position);
                        break;
                    }
                    case POTIONM: {
                    	world_->AddManaPotion(position);
                        world_->AddFloor(position);
                        break;
                    }
                    case POTIONS: {
                    	world_->AddSightPotion(position);
                        world_->AddFloor(position);
                        break;
                    }
                }
            }
        }
    }
	InitializeWallTypes(wall_matrix);
}

} // namespace utils
