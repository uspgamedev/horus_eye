#include "game/map/loader.h"

#include <vector>
#include <list>
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
#include "game/map/giantfloor.h"
#include "game/scenes/world.h"
#include "game/core/coordinates.h"

namespace map {

using std::string;
using std::vector;
using std::list;
using ugdk::math::Vector2D;
using ugdk::math::Integer2D;
using ugdk::script::VirtualObj;
using pyramidworks::collision::CollisionManager;

typedef std::vector<std::string> ArgumentList;

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

struct ObjectDescriptor {
    ObjectDescriptor() {}
    ObjectDescriptor(const string& _type, const ArgumentList& _arguments,
                     const Vector2D& _position, const string& _tag)
                     : type(_type), arguments(_arguments), position(_position), tag(_tag) {}

    string type;
    ArgumentList arguments;
    Vector2D position;
    string tag;
};

namespace {
Room* DoLoadRoom(const string& name, const VirtualObj& room_data, const ugdk::math::Integer2D& position) {

    if(!room_data) return NULL;

    if(!room_data["width"] || !room_data["height"]) return NULL;

    int width = room_data["width"].value<int>();
    int height = room_data["height"].value<int>();
    std::list<ObjectDescriptor> objects;
    
    CollisionManager* collision_manager = WORLD()->collision_manager();
    
    if(room_data["collision_classes"]) {
        VirtualObj::Vector collision_classes = room_data["collision_classes"].value<VirtualObj::Vector>();

        for(VirtualObj::Vector::iterator it = collision_classes.begin(); it != collision_classes.end(); ++it) {
            VirtualObj::Vector collclass = it->value<VirtualObj::Vector>();
            if (collclass.size() >= 2)
                collision_manager->Generate(collclass.front().value<string>(), collclass[1].value<string>());
            else if (collclass.size() >= 1)
                collision_manager->Generate(collclass.front().value<string>());
        }
    }

    Room* room = new Room(name, Integer2D(width, height), position);

    //=========================================
    //         LOADING MATRIX

    if(room_data["matrix"]) {
        vector< vector< ArgumentList > > arguments(height, vector<ArgumentList>(width));
        parseArguments(arguments, room_data["arguments"]);

        vector< vector< std::string > > tags(height, vector<string>(width));
        parseTags(tags, room_data["tags"]);

        string matrix = room_data["matrix"].value<std::string>();
        int y = 0, x = 0;
        for(string::iterator it = matrix.begin(); it != matrix.end(); ++it) {
            if(*it == '\n' || x == width) {
                //TODO if(x != width) { } (tratar erro?)
                x = 0;
                ++y;
                if(y == height) break;
                continue;
            }
            {
                Tile tile(y, x, *it);
                ObjectDescriptor descriptor(string(1, tile.object()), arguments[y][x], Vector2D(x, y), tags[y][x]);
                objects.push_back(descriptor);

                /*if(tile.has_floor()) {
                    ugdk::graphic::Node* floor = new ugdk::graphic::Node(new GiantFloor(room->size()));
                    floor->geometry().set_offset(core::FromWorldCoordinates(descriptor.position));
                    room->floor()->AddChild(floor);
                }*/
            }

            ++x;
        }
    }
    room->floor()->set_drawable(new GiantFloor(room->size()));
    
    //=========================================
    //         LOADING OBJECTS

    if(room_data["objects"]) {
        // Field 1: x
        // Field 2: y
		// Field 3: type string
		// Field 4: arguments list (optional)
		// Field 5: tag (optional)
        VirtualObj::Vector vobj_objects = room_data["objects"].value<VirtualObj::Vector>();

        //ofr object in object list add object hzuzzah
        for (VirtualObj::Vector::iterator it = vobj_objects.begin(); it != vobj_objects.end(); ++it ) {
            VirtualObj::Vector object = it->value<VirtualObj::Vector>();
            if(object.size() < 3 ) {
                printf("Warning: not enough arguments in an object in room '%s'\n", name.c_str());
                continue;
            }
            ObjectDescriptor descriptor;
            descriptor.position = Vector2D(object[0].value<double>(), object[1].value<double>());
            descriptor.type = object[2].value<string>();
            if(object.size() >= 4) {
                VirtualObj::Vector arguments_vobj = object[3].value<VirtualObj::Vector>();
                for(VirtualObj::Vector::iterator it = arguments_vobj.begin(); it != arguments_vobj.end(); ++it)
                    descriptor.arguments.push_back(it->value<std::string>());
            }
            if(object.size() >= 5)
                descriptor.tag = object[4].value<string>();
            objects.push_back(descriptor);
        }
    }

    //=========================================
    //         CREATING OBJECTS

    for(list<ObjectDescriptor>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
        sprite::WorldObject* obj = builder::WorldObjectFromTypename(it->type, it->arguments);
        if(obj) {
            if(!it->tag.empty())
                obj->set_tag(it->tag);
            room->AddObject(obj, it->position);
        } else if(builder::HasFactoryMethod(it->type)) {
            fprintf(stderr, "Warning: unable to create object of type '%s' at (%f;%f) with args {", 
                it->type.c_str(), it->position.x, it->position.y);
            for(ArgumentList::const_iterator arg = it->arguments.begin();arg != it->arguments.end(); ++arg)
                fprintf(stderr, "'%s', ", arg->c_str());
            fprintf(stderr, "}.\n");
        }
    }

    //=========================================
    //         RUNNING SETUP
    
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

} //namespace anon

Room* LoadRoom(const std::string& name, const ugdk::math::Integer2D& position) {
    VirtualObj room_data = SCRIPT_MANAGER()->LoadModule("levels.rooms." + name);
    return DoLoadRoom(name, room_data, position);
}

Room* LoadRoom(const std::string& name, const VirtualObj& room_script, const ugdk::math::Integer2D& position) {
    return DoLoadRoom(name, room_script, position);
}

} // namespace utils
