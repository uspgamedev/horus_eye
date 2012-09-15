#include "game/map/loader.h"

#include <vector>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision/collisionmanager.h>

#include "game/builders/builder.h"
#include "game/sprites/worldobject.h"
#include "game/map/tile.h"
#include "game/map/room.h"
#include "game/scenes/world.h"

namespace map {

using std::string;
using std::vector;
using ugdk::Vector2D;
using ugdk::math::Integer2D;
using ugdk::script::VirtualObj;
using pyramidworks::collision::CollisionManager;

typedef std::vector<std::string> ArgumentList;

ugdk::graphic::Node* BuildFloor(const ugdk::Vector2D& position);

static void parseArguments(vector< vector< ArgumentList > >& args_matrix, VirtualObj vobj) {
    if(vobj) return;
    VirtualObj::Vector arguments = vobj.value<VirtualObj::Vector>();
    for (VirtualObj::Vector::iterator it = arguments.begin(); it != arguments.end(); ++it) {
        VirtualObj::Vector data = it->value<VirtualObj::Vector>();
        int x = data[0].value<int>();
        int y = data[1].value<int>();
        for (VirtualObj::Vector::iterator arg_it = data.begin()+2; arg_it != data.end(); ++arg_it)
            args_matrix[y][x].push_back(arg_it->value<string>());
    }
}

static void parseTags(vector< vector< std::string > >& tags_matrix, VirtualObj vobj) {
    if(vobj) return;
    VirtualObj::Vector arguments = vobj.value<VirtualObj::Vector>();
    for (VirtualObj::Vector::iterator it = arguments.begin(); it != arguments.end(); ++it) {
        VirtualObj::Vector data = it->value<VirtualObj::Vector>();
        int x = data[0].value<int>();
        int y = data[1].value<int>();
        for (VirtualObj::Vector::iterator arg_it = data.begin()+2; arg_it != data.end(); ++arg_it)
            tags_matrix[y][x] = data[2].value<std::string>();
    }
}

Room* LoadRoom(const std::string& name) {
    VirtualObj room_data = SCRIPT_MANAGER()->LoadModule("rooms." + name);
    if(!room_data) return NULL;

    if(!room_data["width"] || !room_data["height"] || !room_data["matrix"]) return false;

    int width = room_data["width"].value<int>();
    int height = room_data["height"].value<int>();
    
    vector< vector< ArgumentList > > arguments(height, vector<ArgumentList>(width));
    parseArguments(arguments, room_data["arguments"]);

    vector< vector< std::string > > tags(height, vector<string>(width));
    parseTags(tags, room_data["tags"]);

    std::string matrix = room_data["matrix"].value<std::string>();

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

    CollisionManager* collision_manager = WORLD()->collision_manager();
    
    VirtualObj::Vector collision_classes;
    if(room_data["collision_classes"])
        collision_classes = room_data["collision_classes"].value<VirtualObj::Vector>();

    for(VirtualObj::Vector::iterator it = collision_classes.begin(); it != collision_classes.end(); ++it) {
        VirtualObj::Vector collclass = it->value<VirtualObj::Vector>();
        if (collclass.size() >= 2)
            collision_manager->Generate(collclass.front().value<string>(), collclass[1].value<string>());
        else if (collclass.size() >= 1)
            collision_manager->Generate(collclass.front().value<string>());
    }

    Room* room = new Room(Integer2D(width, height), gamemap);
    for (int i = 0; i < (int)gamemap.size(); ++i) {
        for (int j = 0; j < (int)gamemap[i].size(); ++j) {
            Vector2D position((double) j, (double) i);

            sprite::WorldObject* obj = builder::WorldObjectFromTypename(string(1, gamemap[i][j]->object()), arguments[i][j]);
            if(obj) {
                obj->set_tag(tags[i][j]);
                room->AddObject(obj, position);
            }


            if(gamemap[i][j]->has_floor()) {
                //ugdk::graphic::Node* floor = BuildFloor();
                // TODO set position for floor
            }
        }
    }

    VirtualObj::Vector objects;
    if(room_data["objects"])
        objects = room_data["objects"].value<VirtualObj::Vector>();

    //ofr object in object list add object hzuzzah
    for (VirtualObj::Vector::iterator it = objects.begin(); it != objects.end(); ++it ) {
        VirtualObj::Vector object = it->value<VirtualObj::Vector>();
        if (object.size() < 3){
            printf("Warning: not enough arguments in an object in room '%s'\n", name.c_str());
            continue;
        }
        double x = object[0].value<double>();
        double y = object[1].value<double>();
        string objecttype = object[2].value<string>();
        ArgumentList args;
        for(size_t i = 3; i < object.size(); ++i)
            args.push_back(object[i].value<std::string>());
        sprite::WorldObject* obj = builder::WorldObjectFromTypename(objecttype, args);
        if(obj)
            room->AddObject(obj, Vector2D(x,y));
    }
    
    VirtualObj setup = room_data["setup"];
    if (setup) 
        setup();

    return room;
}

} // namespace utils
