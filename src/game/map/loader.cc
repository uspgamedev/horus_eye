#include "game/map/loader.h"

#include <vector>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/node.h>
#include <pyramidworks/collision/collisionmanager.h>

#include "game/builders/builder.h"
#include "game/builders/doodadbuilder.h"
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
    if(!vobj) return;
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
    if(!vobj) return;
    VirtualObj::Vector arguments = vobj.value<VirtualObj::Vector>();
    for (VirtualObj::Vector::iterator it = arguments.begin(); it != arguments.end(); ++it) {
        VirtualObj::Vector data = it->value<VirtualObj::Vector>();
        int x = data[0].value<int>();
        int y = data[1].value<int>();
        tags_matrix[y][x] = data[2].value<std::string>();
    }
}

Room* LoadRoom(const std::string& name, const ugdk::math::Integer2D& position) {
    VirtualObj room_data = SCRIPT_MANAGER()->LoadModule("rooms." + name);
    if(!room_data) return NULL;

    if(!room_data["width"] || !room_data["height"] || !room_data["matrix"]) return NULL;

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

    Room* room = new Room(name, Integer2D(width, height), position, gamemap);
    for (int i = 0; i < (int)gamemap.size(); ++i) {
        for (int j = 0; j < (int)gamemap[i].size(); ++j) {
            if(gamemap[i][j] == NULL) {
                gamemap[i][j] = new Tile(i, j);
            }

            Vector2D position((double) j, (double) i);

            std::string type_string(1, gamemap[i][j]->object());
            sprite::WorldObject* obj = builder::WorldObjectFromTypename(type_string, arguments[i][j]);
            if(obj) {
                obj->set_tag(tags[i][j]);
                room->AddObject(obj, position);
            } else if(builder::HasFactoryMethod(type_string)) {
                fprintf(stderr, "Warning: unable to create object of type '%s' from matrix (%d;%d) with args {", 
                    type_string.c_str(), j, i);
                for(ArgumentList::const_iterator it = arguments[i][j].begin(); it != arguments[i][j].end(); ++it)
                    fprintf(stderr, "'%s', ", it->c_str());
                fprintf(stderr, "}.\n");
            }

            if(gamemap[i][j]->has_floor()) {
                ugdk::graphic::Node* floor = builder::DoodadBuilder::Floor(position);
                room->floor()->AddChild(floor);
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
        if(object.size() >= 4) {
            VirtualObj::Vector arguments_vobj = object[3].value<VirtualObj::Vector>();
            for(VirtualObj::Vector::iterator it = arguments_vobj.begin(); it != arguments_vobj.end(); ++it)
                args.push_back(it->value<std::string>());
        }
        sprite::WorldObject* obj = builder::WorldObjectFromTypename(objecttype, args);
        if(obj) {
            if(object.size() >= 5)
                obj->set_tag(object[4].value<std::string>());
            room->AddObject(obj, Vector2D(x,y));
        } else if(builder::HasFactoryMethod(objecttype)) {
            fprintf(stderr, "Warning: unable to create object of type '%s' at (%f;%f) with args {", 
                objecttype.c_str(), x, y);
            for(ArgumentList::const_iterator it = args.begin(); it != args.end(); ++it)
                fprintf(stderr, "'%s', ", it->c_str());
            fprintf(stderr, "}.\n");
        }
    }
    
    VirtualObj setup = room_data["setup"];
    if (setup) {
        VirtualObj room_vobj(setup.wrapper());
        room_vobj.set_value<Room*>(room);

        VirtualObj::List args;
        args.push_back(room_vobj);
        setup(args);
    }

    return room;
}

} // namespace utils