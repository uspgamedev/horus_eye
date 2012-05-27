#include <algorithm>
#include <functional>
#include <locale>
#include <cstdio>
#include <cfloat>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/node.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/audio/audiomanager.h>

#include "levelloader.h"

#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/sprites/scenery/wall.h"
#include "game/sprites/scenery/block.h"
#include "game/sprites/scenery/button.h"
#include "game/builders/itembuilder.h"
#include "game/builders/mummybuilder.h"
#include "game/builders/entitybuilder.h"
#include "game/utils/imagefactory.h"
#include "game/utils/tile.h"


/* Util functions found at http://stackoverflow.com/q/217605 */
// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(isspace))));
    return s;
}
// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}
// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

namespace utils {

using namespace std;
using namespace scene;
using namespace sprite;
using namespace ugdk;

#define LINE_SIZE 1024

void LevelLoader::LoadMatrix(string file_name) {
    FILE *file = fopen(PATH_MANAGER()->ResolvePath(file_name).c_str(), "r");

    if(file != NULL) {
        char buffer[LINE_SIZE];
        fgets(buffer, LINE_SIZE, file);
        string music(buffer);
        trim(music);

        fgets(buffer, LINE_SIZE, file);
        int width, height;
        sscanf(buffer, "%d %d", &width, &height);

        GameMap matrix(height);

        for (int i = 0; i < height; ++i) {
            fgets(buffer, LINE_SIZE, file);
            matrix[i] = TileRow(width);
            for (int j = 0; j < width; j++) {
                matrix[i][j] = new Tile(i, j, buffer[j]);
            }
        }

        world_->set_background_music(AUDIO_MANAGER()->LoadMusic(music));
        world_->set_level_width(width);
        world_->set_level_height(height);
        world_->set_level_matrix(matrix);

        fclose(file);
    } else {
        fprintf(stdout, "CANNOT OPEN FILE: %s\n", file_name.c_str());
        exit(1);
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

void LevelLoader::TokenToWorldObject(char token, int i, int j, const Vector2D& position, vector<vector<Wall* > > &wall_matrix) {
    builder::MummyBuilder mummy_builder;
    builder::ItemBuilder potion_builder;
    builder::EntityBuilder entity_builder;
    ImageFactory* image_factory = world_->image_factory();
    if(token != EMPTY) {
        switch(token) {
            case WALL: {
                wall_matrix[i][j] = new Wall(image_factory->WallImage());
                world_->AddWorldObject(wall_matrix[i][j], position);
                break;
            }
            case ENTRY: {
                wall_matrix[i][j] = new Wall(image_factory->EntryImage());
                world_->AddWorldObject(wall_matrix[i][j], position);
                break;
            }
            case BLOCK: {
                world_->AddWorldObject(new Block(image_factory->WallImage()), position);
                break;
            }
            case HERO: {
                world_->AddHero(position);
                break;
            }
            case STANDING_MUMMY: {
                world_->AddWorldObject(mummy_builder.StandingMummy(image_factory->MummyImage()), position);
                world_->IncreaseNumberOfEnemies();
                break;
            }
            case MUMMY: {
                world_->AddWorldObject(mummy_builder.WalkingMummy(image_factory->MummyImage()), position);
                world_->IncreaseNumberOfEnemies();
                break;
            }
            case BIG_MUMMY: {
                world_->AddWorldObject(mummy_builder.BigMummy(image_factory->BigMummyImage()), position);
                world_->IncreaseNumberOfEnemies();
                break;
            }
            case STANDING_BIG_MUMMY: {
                world_->AddWorldObject(mummy_builder.StandingBigMummy(image_factory->BigMummyImage()), position);
                world_->IncreaseNumberOfEnemies();
                break;
            }
            case RANGED_MUMMY: {
                world_->AddWorldObject(mummy_builder.RangedMummy(image_factory->RangedMummyImage()), position);
                world_->IncreaseNumberOfEnemies();
                break;
            }
            case STANDING_RANGED_MUMMY: {
                world_->AddWorldObject(mummy_builder.StandingRangedMummy(image_factory->RangedMummyImage()), position);
                world_->IncreaseNumberOfEnemies();
                break;
            }
            case PHARAOH: {
                world_->AddWorldObject(mummy_builder.WalkingPharaoh(image_factory->PharaohImage()), position);
                world_->IncreaseNumberOfEnemies();
                break;
            }
            case STANDING_PHARAOH: {
                world_->AddWorldObject(mummy_builder.StandingPharaoh(image_factory->PharaohImage()), position);
                world_->IncreaseNumberOfEnemies();
                break;
            }
            case DOOR: {
                GameMap& matrix = world_->level_matrix();
                if(j < world_->level_width()-1 && matrix[i][j+1]->object() == DOOR) {
                    Vector2D pos = position + Vector2D(0.5, 0);
                    world_->AddWorldObject(entity_builder.Door(world_), pos);
                }
                break;
            }
            case FLOOR: { break; }
            case POTIONL: {
                world_->AddWorldObject(potion_builder.LifePotion(image_factory->LifePotionImage()), position);
                break;
            }
            case POTIONM: {
                world_->AddWorldObject(potion_builder.ManaPotion(image_factory->ManaPotionImage()), position);
                break;
            }
            case POTIONS: {
                world_->AddWorldObject(potion_builder.SightPotion(image_factory->SightPotionImage()), position);
                break;
            }
            case BLUEGEM: {
                world_->AddWorldObject(potion_builder.BlueGem(image_factory->BlueGemImage()), position);
                break;
            }
            case BUTTON: {
                world_->AddWorldObject(new Button(image_factory->TileSwitchImage(), world_, -1), position);
                break;
            }
        }
    }
}

void LevelLoader::Load(string file_name) {

    LoadMatrix(file_name);
    GameMap& matrix = world_->level_matrix();
    world_->SetupCollisionManager();

    vector<vector<Wall* > > wall_matrix(matrix.size(), vector<Wall *> (matrix[0].size()));

    ugdk::graphic::Node* floors = new ugdk::graphic::Node;

    for (int i = 0; i < (int)matrix.size(); ++i) {
        for (int j = 0; j < (int)matrix[i].size(); ++j) {
            char token = matrix[i][j]->object();
            Vector2D position ((double)j, (double)(world_->level_height() - i - 1));

            if(token != EMPTY) {
                ugdk::graphic::Node* floor = matrix[i][j]->floor();
                floor->set_drawable(world_->image_factory()->FloorImage());
                floor->modifier()->set_offset(World::FromWorldCoordinates(position));
                floor->modifier()->set_color(Color(0.5, 0.5, 0.5));
                floors->AddChild(floor);
            }

            TokenToWorldObject(token, i, j, position, wall_matrix);
        }
    }
    InitializeWallTypes(wall_matrix);
    world_->content_node()->AddChild(floors);
    floors->set_zindex(-FLT_MAX);
}

} // namespace utils
