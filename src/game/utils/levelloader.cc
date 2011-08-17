#include <fstream>
#include <iostream>
#include "levelloader.h"
#include "../../framework/engine.h"
#include "../../framework/pathmanager.h"
#include "../sprites/worldobject.h"
#include "../sprites/hero.h"
#include "../sprites/mummy.h"
#include "../sprites/floor.h"
#include "../sprites/door.h"
#include "../sprites/wall.h"
#include "../sprites/itembuilder.h"
#include "../sprites/mummybuilder.h"
#include "imagefactory.h"
#include "tile.h"

namespace utils {

using namespace std;
using namespace scene;
using namespace sprite;
using namespace ugdk;

void LevelLoader::LoadMatrix(string file_name) {
	ifstream file (PATH_MANAGER()->ResolvePath(file_name).c_str());

	if(file.is_open()){
		string music;
		file >> music;
		int width, height;
		file >> width >> height; 
		vector<string> raw_matrix (height);
		GameMap matrix(height);

		for (int i = 0; i < height; ++i) {
			file >> raw_matrix[i];
			matrix[i] = TileRow(width);
			for (int j = 0; j < width; j++) {
			    matrix[i][j] = new Tile(i, j, raw_matrix[i][j]);
			}
		}

		world_->set_music(music);
		world_->set_level_width(width);
		world_->set_level_height(height);
		world_->set_level_matrix(matrix);

		file.close();
	} else {
		cout << "CANNOT OPEN FILE: " << file_name << endl;
		exit(0);
	}
}

bool LevelLoader::InRange (int i,int j) {
	if(i >= world_->level_height() || j >= world_->level_width() || i < 0 || j < 0)
		return false;
	return true;
}

bool LevelLoader::IsWall(int i, int j) {
	return InRange(i,j) ? world_->level_matrix()[i][j]->object() == WALL : false;
}

void LevelLoader::InitializeWallTypes(vector<vector<Wall *> > wall_matrix) {
	GameMap& matrix = world_->level_matrix();
	
	for (int i = 0; i < (int)matrix.size(); ++i) {
		for (int j = 0; j < (int)matrix[i].size(); ++j) {
			if(matrix[i][j]->object() == WALL) {
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

void LevelLoader::TokenToWorldObject(char token, int i, int j, Vector2D position, vector<vector<Wall* > > &wall_matrix) {
    MummyBuilder mummy_builder;
    ItemBuilder potion_builder;
    ImageFactory* image_factory = world_->image_factory();
	if(token != EMPTY) {
		switch(token) {
			case WALL: {
				wall_matrix[i][j] = new Wall(image_factory->WallImage());
				world_->AddWorldObject(wall_matrix[i][j], position);
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case ENTRY: {
				wall_matrix[i][j] = new Wall(image_factory->EntryImage());
				world_->AddWorldObject(wall_matrix[i][j], position);
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case HERO: {
				world_->AddHero(position);
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case STANDING_MUMMY: {
				world_->AddWorldObject(mummy_builder.StandingMummy(image_factory->MummyImage()), position);
				world_->IncreaseNumberOfEnemies();
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case MUMMY: {
				world_->AddWorldObject(mummy_builder.WalkingMummy(image_factory->MummyImage()), position);
				world_->IncreaseNumberOfEnemies();
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case BIG_MUMMY: {
				world_->AddWorldObject(mummy_builder.BigMummy(image_factory->BigMummyImage()), position);
				world_->IncreaseNumberOfEnemies();
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case STANDING_BIG_MUMMY: {
				world_->AddWorldObject(mummy_builder.StandingBigMummy(image_factory->BigMummyImage()), position);
				world_->IncreaseNumberOfEnemies();
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case RANGED_MUMMY: {
				world_->AddWorldObject(mummy_builder.RangedMummy(image_factory->RangedMummyImage()), position);
				world_->IncreaseNumberOfEnemies();
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case STANDING_RANGED_MUMMY: {
				world_->AddWorldObject(mummy_builder.StandingRangedMummy(image_factory->RangedMummyImage()), position);
				world_->IncreaseNumberOfEnemies();
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case PHARAOH: {
				world_->AddWorldObject(mummy_builder.WalkingPharaoh(image_factory->PharaohImage()), position);
				world_->IncreaseNumberOfEnemies();
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case STANDING_PHARAOH: {
				world_->AddWorldObject(mummy_builder.StandingPharaoh(image_factory->PharaohImage()), position);
				world_->IncreaseNumberOfEnemies();
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case DOOR: {
				GameMap& matrix = world_->level_matrix();
				if(j < world_->level_width()-1 && matrix[i][j+1]->object() == DOOR) {
					Vector2D pos = position + Vector2D(0.5, 0);
					world_->AddWorldObject(new Door(image_factory->DoorImage()), pos);

				}
			//Sembreakdeproposito.
			}
			case FLOOR: {
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case POTIONL: {
				world_->AddWorldObject(potion_builder.LifePotion(image_factory->LifePotionImage()), position);
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case POTIONM: {
				world_->AddWorldObject(potion_builder.ManaPotion(image_factory->ManaPotionImage()), position);
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
			case POTIONS: {
				world_->AddWorldObject(potion_builder.SightPotion(image_factory->SightPotionImage()), position);
				world_->AddWorldObject(new Floor(image_factory->FloorImage()), position);
				break;
			}
		}
	}
}

void LevelLoader::Load(string file_name) {

	LoadMatrix(file_name);
	GameMap& matrix = world_->level_matrix();

	vector<vector<Wall* > > wall_matrix(matrix.size(), vector<Wall *> (matrix[0].size()));

	for (int i = 0; i < (int)matrix.size(); ++i) {
		for (int j = 0; j < (int)matrix[i].size(); ++j) {
			char token = matrix[i][j]->object();
			Vector2D position ((float)j, (float)(world_->level_height() - i - 1));

			TokenToWorldObject(token, i, j, position, wall_matrix);
		}
	}
	InitializeWallTypes(wall_matrix);
}

} // namespace utils
