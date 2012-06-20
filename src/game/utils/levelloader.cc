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
#include "game/components/logic/wall.h"
#include "game/utils/imagefactory.h"
#include "game/utils/tile.h"
#include "game/utils/settings.h"

#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>


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
using component::Wall;
using ugdk::script::VirtualObj;

#define LINE_SIZE 1024

bool LevelLoader::LoadMatrix(const std::string& file_name) {

	VirtualObj level_data = SCRIPT_MANAGER()->LoadModule("levels." + file_name);
	if(!level_data) return false;

	//TODO: error checking
	std::string music_name = level_data["music"].value<std::string>();
	int width = level_data["width"].value<int>();
	int height = level_data["height"].value<int>();
	
	arguments_.resize(height, std::vector<std::string>(width));

	VirtualObj::Vector arguments = level_data["arguments"].value<VirtualObj::Vector>();
	for (VirtualObj::Vector::iterator it = arguments.begin(); it != arguments.end(); ++it) {
		
		VirtualObj::Vector data = it->value<VirtualObj::Vector>();
		int x = data[0].value<int>();
		int y = data[1].value<int>();
		arguments_[y][x] = data[2].value<std::string>();
		printf("x=%d; y=%d; arg='%s'\n", x, y, arguments_[y][x].c_str());
	}

	std::string matrix = level_data["matrix"].value<std::string>();

	GameMap gamemap(height, TileRow(width));
	{
		int y = 0, x = 0;
		for(std::string::iterator it = matrix.begin(); it != matrix.end(); ++it) {
			if(*it == '\n') {
				//TODO if(x != width) { } (tratar erro?)
				x = 0;
				++y;
				if(y == height) break;
				continue;
			}
			gamemap[y][x] = new Tile(y, x, *it);
			++x;
		}
	}
	if(Settings::reference()->background_music())
		world_->set_background_music(AUDIO_MANAGER()->LoadMusic(music_name));
	world_->set_level_width(width);
	world_->set_level_height(height);
	world_->set_level_matrix(gamemap);
	return true;
}

bool LevelLoader::InRange (int i,int j) {
    if(i >= world_->level_height() || j >= world_->level_width() || i < 0 || j < 0)
        return false;
    return true;
}

bool LevelLoader::IsWall(int i, int j) {
    return InRange(i,j) ? world_->level_matrix()[i][j]->object() == WALL : false;
}

void LevelLoader::InitializeWallTypes() {
    GameMap& matrix = world_->level_matrix();
    
    for (int i = 0; i < (int)matrix.size(); ++i) {
        for (int j = 0; j < (int)matrix[i].size(); ++j) {
            if(matrix[i][j]->object() == WALL) {
                if(IsWall(i, j-1)) {
                    if(IsWall(i+1, j)) {
                        wall_matrix_[i][j]->set_type(Wall::MIDDLE);
                    } else {
                        wall_matrix_[i][j]->set_type(Wall::RIGHT);
                    }
                } else {
                    if(IsWall(i+1, j)) {
                        wall_matrix_[i][j]->set_type(Wall::BOTTOM);
                    } else {
                        wall_matrix_[i][j]->set_type(Wall::BOTTOMRIGHT);
                    }
                }
            }
        }
    }
}

WorldObject* LevelLoader::GenerateBlock(const std::string&) {
	return doodad_builder_.Block();
}
WorldObject* LevelLoader::GenerateStandingMummy(const std::string&) {
	return mummy_builder_.StandingMummy(world_->image_factory()->MummyImage());
}
WorldObject* LevelLoader::GenerateMummy(const std::string&) {
	return mummy_builder_.WalkingMummy(world_->image_factory()->MummyImage());
}
WorldObject* LevelLoader::GenerateStandingBigMummy(const std::string&) {
	return mummy_builder_.StandingBigMummy(world_->image_factory()->BigMummyImage());
}
WorldObject* LevelLoader::GenerateBigMummy(const std::string&) {
	return mummy_builder_.BigMummy(world_->image_factory()->BigMummyImage());
}
WorldObject* LevelLoader::GenerateStandingRangedMummy(const std::string&) {
	return mummy_builder_.StandingRangedMummy(world_->image_factory()->RangedMummyImage());
}
WorldObject* LevelLoader::GenerateRangedMummy(const std::string&) {
	return mummy_builder_.RangedMummy(world_->image_factory()->RangedMummyImage());
}
WorldObject* LevelLoader::GenerateStandingPharaoh(const std::string&) {
	return mummy_builder_.StandingPharaoh(world_->image_factory()->PharaohImage());
}
WorldObject* LevelLoader::GeneratePharaoh(const std::string&) {
	return mummy_builder_.WalkingPharaoh(world_->image_factory()->PharaohImage());
}
WorldObject* LevelLoader::GenerateLifePotion(const std::string&) {
	return potion_builder_.LifePotion(world_->image_factory()->LifePotionImage());
}
WorldObject* LevelLoader::GenerateManaPotion(const std::string&) {
	return potion_builder_.ManaPotion(world_->image_factory()->ManaPotionImage());
}
WorldObject* LevelLoader::GenerateSightPotion(const std::string&) {
	return potion_builder_.SightPotion(world_->image_factory()->SightPotionImage());
}
WorldObject* LevelLoader::GenerateBlueGem(const std::string&) {
	return potion_builder_.BlueGem(world_->image_factory()->BlueGemImage());
}
WorldObject* LevelLoader::GenerateButton(const std::string& arg) {
	world_->num_button_not_pressed() += 1;
	return doodad_builder_.Button(arg);
}

void LevelLoader::TokenToWorldObject(char token, int i, int j, const Vector2D& position) {
	switch(token) {
		case WALL: {
			WorldObject* wobj = doodad_builder_.Wall();
			wall_matrix_[i][j] = static_cast<Wall*>(wobj->logic());
			world_->AddWorldObject(wobj, position);
			break;
		}
		case ENTRY: {
			WorldObject* wobj = doodad_builder_.Entry();
			wall_matrix_[i][j] = static_cast<Wall*>(wobj->logic());
			world_->AddWorldObject(wobj, position);
			break;
		}
		case HERO: {
			world_->AddHero(position);
			break;
		}
		case DOOR: {
			GameMap& matrix = world_->level_matrix();
			if(j < world_->level_width()-1 && matrix[i][j+1]->object() == DOOR) {
				Vector2D pos = position + Vector2D(0.5, 0);
				world_->AddWorldObject(doodad_builder_.Door(world_), pos);
			}
			break;
		}
		default: {
		}
	}
}

void LevelLoader::Load(const std::string& file_name) {

	bool load_success = LoadMatrix(file_name);

    world_->SetupCollisionManager();
    if(!load_success) return;

	GameMap& matrix = world_->level_matrix();

    vector<vector<Wall* > > wall_matrix(matrix.size(), vector<Wall *> (matrix[0].size()));
    wall_matrix_ = wall_matrix;

    ugdk::graphic::Node* floors = new ugdk::graphic::Node;

    token_function_[BLOCK] = &LevelLoader::GenerateBlock;
    token_function_[STANDING_MUMMY] = &LevelLoader::GenerateStandingMummy;
    token_function_[STANDING_BIG_MUMMY] = &LevelLoader::GenerateStandingBigMummy;
    token_function_[STANDING_RANGED_MUMMY] = &LevelLoader::GenerateStandingRangedMummy;
    token_function_[STANDING_PHARAOH] = &LevelLoader::GenerateStandingPharaoh;
    token_function_[MUMMY] = &LevelLoader::GenerateMummy;
    token_function_[BIG_MUMMY] = &LevelLoader::GenerateBigMummy;
    token_function_[RANGED_MUMMY] = &LevelLoader::GenerateRangedMummy;
    token_function_[PHARAOH] = &LevelLoader::GeneratePharaoh;
    token_function_[POTIONL] = &LevelLoader::GenerateLifePotion;
    token_function_[POTIONM] = &LevelLoader::GenerateManaPotion;
    token_function_[POTIONS] = &LevelLoader::GenerateSightPotion;
    token_function_[BLUEGEM] = &LevelLoader::GenerateBlueGem;
    token_function_[BUTTON] = &LevelLoader::GenerateButton;

    for (int i = 0; i < (int)matrix.size(); ++i) {
        for (int j = 0; j < (int)matrix[i].size(); ++j) {
        	Vector2D position ((double)j, (double)(world_->level_height() - i - 1));

        	char token = matrix[i][j]->object();
        	if(token_function_[token]) {
				WorldObject* obj = token_function_[token](this, arguments_[i][j]);
				if(obj) {
					world_->AddWorldObject(obj, position);
				}
        	} else {
        		TokenToWorldObject(token, i, j, position);
        	}
            if(matrix[i][j]->has_floor()) {
                ugdk::graphic::Node* floor = matrix[i][j]->floor();
                floor->set_drawable(world_->image_factory()->FloorImage());
                floor->modifier()->set_offset(World::FromWorldCoordinates(position));
                floor->modifier()->set_color(Color(0.5, 0.5, 0.5));
                floors->AddChild(floor);
            }
        }
    }
    InitializeWallTypes();
    world_->content_node()->AddChild(floors);
    floors->set_zindex(-FLT_MAX);
}

} // namespace utils
