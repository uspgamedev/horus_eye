#include <algorithm>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include <locale>
#include <cstdio>
#include <cfloat>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/node.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>

#include "levelloader.h"

#include "game/builders/scriptbuilder.h"
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
using ugdk::script::VirtualObj;
using std::tr1::bind;
using namespace std::tr1::placeholders;

#define LINE_SIZE 1024

bool LevelLoader::LoadMatrix(const std::string& file_name) {

    VirtualObj level_data = SCRIPT_MANAGER()->LoadModule("levels." + file_name);
    if(!level_data) return false;

    if(!level_data["width"] || !level_data["height"] || !level_data["matrix"]) return false;

    std::string music_name = level_data["music"] ? level_data["music"].value<std::string>() : "";
    int width = level_data["width"].value<int>();
    int height = level_data["height"].value<int>();
    
    {
        std::vector<ArgumentList> line(width);
        arguments_.resize(height, line);

        if(level_data["arguments"]) {
            VirtualObj::Vector arguments = level_data["arguments"].value<VirtualObj::Vector>();
            for (VirtualObj::Vector::iterator it = arguments.begin(); it != arguments.end(); ++it) {
                VirtualObj::Vector data = it->value<VirtualObj::Vector>();
                int x = data[0].value<int>();
                int y = data[1].value<int>();
                arguments_[y][x].push_back(data[2].value<std::string>());
            }
        }
    }
    {
        std::vector<string> line(width);
        tags_.resize(height, line);

        if(level_data["tags"]) {
            VirtualObj::Vector tags = level_data["tags"].value<VirtualObj::Vector>();
            for (VirtualObj::Vector::iterator it = tags.begin(); it != tags.end(); ++it) {
                VirtualObj::Vector data = it->value<VirtualObj::Vector>();
                int x = data[0].value<int>();
                int y = data[1].value<int>();
                tags_[y][x] = data[2].value<std::string>();
            }
        }
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

    if(level_data["objects"])
        objects_ = level_data["objects"].value<VirtualObj::Vector>();

    if(Settings::reference()->background_music() && music_name.length() > 0)
        world_->set_background_music(AUDIO_MANAGER()->LoadMusic(music_name));
    world_->set_level_width(width);
    world_->set_level_height(height);
    world_->set_level_matrix(gamemap);
    setup_ = level_data["setup"];
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

void LevelLoader::TokenToWorldObject(char token, int i, int j, const Vector2D& position) {
    ArgumentList blank;
    switch(token) {
        case WALL: {
            WorldObject* wobj = builder::DoodadBuilder::Wall(blank);
            wobj->set_tag(tags_[i][j]);
            wall_matrix_[i][j] = static_cast<Wall*>(wobj->logic());
            world_->AddWorldObject(wobj, position);
            break;
        }
        case ENTRY: {
            WorldObject* wobj = builder::DoodadBuilder::Entry(blank);
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
                world_->AddWorldObject(builder::DoodadBuilder::Door(blank, world_), pos);
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

    builder::EntityBuilder entity_builder_;

    token_function_[BLOCK] = builder::DoodadBuilder::Block;
    token_function_[STANDING_MUMMY] = builder::MummyBuilder::StandingMummy;
    token_function_[STANDING_BIG_MUMMY] = builder::MummyBuilder::StandingBigMummy;
    token_function_[STANDING_RANGED_MUMMY] = builder::MummyBuilder::StandingRangedMummy;
    token_function_[STANDING_PAPER_MUMMY] = builder::MummyBuilder::StandingPaperMummy;
    token_function_[STANDING_PHARAOH] = builder::MummyBuilder::StandingPharaoh;
    token_function_[MUMMY] = builder::MummyBuilder::WalkingMummy;
    token_function_[BIG_MUMMY] = builder::MummyBuilder::WalkingBigMummy;
    token_function_[RANGED_MUMMY] = builder::MummyBuilder::WalkingRangedMummy;
    token_function_[PHARAOH] = builder::MummyBuilder::WalkingPharaoh;
    token_function_[POTIONL] = builder::ItemBuilder::LifePotion;
    token_function_[POTIONM] = builder::ItemBuilder::ManaPotion;
    token_function_[POTIONS] = builder::ItemBuilder::SightPotion;
    token_function_[BLUEGEM] = builder::ItemBuilder::BlueGem;
    token_function_[BUTTON] = builder::DoodadBuilder::Button; // TODO: world_->num_button_not_pressed() += 1;
    token_function_[SCRIPT] = builder::ScriptBuilder::Script;

    ImageFactory factory;

    for (int i = 0; i < (int)matrix.size(); ++i) {
        for (int j = 0; j < (int)matrix[i].size(); ++j) {
            Vector2D position ((double)j, (double)(world_->level_height() - i - 1));

            char token = matrix[i][j]->object();
            if(token_function_[token]) {
                WorldObject* obj = token_function_[token](arguments_[i][j]);
                if(obj) {
                    obj->set_tag(tags_[i][j]);
                    world_->AddWorldObject(obj, position);
                }
            } else {
                TokenToWorldObject(token, i, j, position);
            }
            if(matrix[i][j]->has_floor()) {
                ugdk::graphic::Node* floor = matrix[i][j]->floor();
                floor->set_drawable(factory.FloorImage());
                floor->modifier()->set_offset(World::FromWorldCoordinates(position));
                floor->modifier()->set_color(Color(0.5, 0.5, 0.5));
                floors->AddChild(floor);
            }
        }
    }
    //ofr object in object list add object hzuzzah
    for (VirtualObj::Vector::iterator it = objects_.begin(); it != objects_.end(); ++it ) {
        VirtualObj::Vector object = it->value<VirtualObj::Vector>();
        if (object.size() < 3){
            printf("dafuq\n");
            continue;
        }
        double x = object[0].value<double>();
        double y = object[1].value<double>();
        char objecttype = object[2].value<string>()[0];
        std::vector<std::string> args;
        for(size_t i = 3; i < object.size(); ++i)
            args.push_back(object[i].value<std::string>());
        WorldObject* obj = token_function_[objecttype](args);
        if(obj){
            world_->AddWorldObject(obj, Vector2D(x,y));
        }
    }
    InitializeWallTypes();
    world_->content_node()->AddChild(floors);
    floors->set_zindex(-FLT_MAX);
    if (setup_)
        setup_();
}

} // namespace utils
