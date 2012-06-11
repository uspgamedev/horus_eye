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

#define LINE_SIZE 1024

bool LevelLoader::LoadMatrix(string file_name) {
    FILE *file = fopen(PATH_MANAGER()->ResolvePath(file_name).c_str(), "r");

    if(file == NULL) {
		fprintf(stderr, "CANNOT OPEN FILE: %s\n", file_name.c_str());
		return false;
    }
	char buffer[LINE_SIZE];
	fgets(buffer, LINE_SIZE, file);
	string music(buffer);
	trim(music);

	fgets(buffer, LINE_SIZE, file);
	int width, height;
	if(sscanf(buffer, "%d %d", &width, &height) < 2) return false;

	GameMap matrix(height);
	arguments_.resize(height);
	for (int i = 0; i < height; ++i)
		arguments_[i].resize(width);

	fgets(buffer, LINE_SIZE, file);
	int num_arguments;
	if(sscanf(buffer, "%d", &num_arguments) < 1) return false;

	for (int i = 0; i < num_arguments; ++i) {
		fgets(buffer, LINE_SIZE, file);
		int x, y;
		if(sscanf(buffer, "%d %d", &x, &y) < 2) return false;
		char* arg = strchr(buffer, ' ');
		arg = strchr(arg + 1, ' ');
		if(!arg || strlen(arg) < 2) return false;
		arg[strlen(arg) - 1] = '\0'; // remove linefeed
		arguments_[y][x] = std::string(arg + 1);
		printf("Buffer '%s', x=%d; y=%d; arg='%s'\n", buffer, x, y, arguments_[y][x].c_str());
	}

	for (int i = 0; i < height; ++i) {
		fgets(buffer, LINE_SIZE, file);
		matrix[i] = TileRow(width);
		for (int j = 0; j < width; j++) {
			matrix[i][j] = new Tile(i, j, buffer[j]);
		}
	}

	if(Settings::reference()->background_music())
		world_->set_background_music(AUDIO_MANAGER()->LoadMusic(music));
	world_->set_level_width(width);
	world_->set_level_height(height);
	world_->set_level_matrix(matrix);

	fclose(file);
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

void LevelLoader::Load(string file_name) {

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
