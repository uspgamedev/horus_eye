#include "levelloader.h"

#include <ugdk/system/engine.h>
#include <ugdk/audio/module.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>

#include "game/builders/herobuilder.h"
#include "game/scenes/world.h"
#include "game/map/loader.h"
#include "game/map/room.h"
#include "game/utils/settings.h"

namespace utils {

using ugdk::script::VirtualObj;
using ugdk::math::Integer2D;
using ugdk::math::Vector2D;

Vector2D VobjsToVector2D(VirtualObj x, VirtualObj y) {
    return Vector2D(x.value<double>(), y.value<double>());
}

void LoadLevel(const VirtualObj& level_data, const std::string& level_path, scene::World** world_ptr) {
    *world_ptr = nullptr;
    SCRIPT_MANAGER()->LoadModule("event") ["ClearAll"] ();
    if(!level_data) return;

    if(!level_data["width"] || !level_data["height"] || !level_data["rooms"] || !level_data["start_position"]) 
        return;

    int width = level_data["width"].value<int>();
    int height = level_data["height"].value<int>();

    scene::World* world = *world_ptr = new scene::World(Integer2D(width, height));

    if(VirtualObj collision_classes = level_data["collision_classes"]) {
        VirtualObj::Vector collision_classes_vector = collision_classes.value<VirtualObj::Vector>();
        for(const VirtualObj& it : collision_classes_vector) {
            VirtualObj::Vector collclass = it.value<VirtualObj::Vector>();
            if (collclass.size() >= 2)
                world->collision_manager()->ChangeClassParent(collclass[0].value<std::string>(), collclass[1].value<std::string>());
        }
    }

    VirtualObj::List rooms = level_data["rooms"].value<VirtualObj::List>();
    for(VirtualObj::List::iterator it = rooms.begin(); it != rooms.end(); ++it) {
        VirtualObj::Vector room_data = it->value<VirtualObj::Vector>();
        if(room_data.size() != 3) continue;
        int x = room_data[0].value<int>();
        int y = room_data[1].value<int>();
        std::string room_name = room_data[2].value<std::string>();

        VirtualObj room_vobj = level_data[room_name].valid()
            ? level_data[room_name]
            : SCRIPT_MANAGER()->LoadModule(level_path + "." + room_name);

        map::Room* room = map::LoadRoom(room_name, room_vobj, Integer2D(x, y), world->collision_manager());
        if(room) {
            world->AddRoom(room);
        } else {
            printf("Room '%s' could not be loaded.\n", room_name.c_str());
        }
    }

    VirtualObj::Vector start_position = level_data["start_position"].value<VirtualObj::Vector>();
    world->set_hero_initial_data(start_position[0].value<std::string>(), VobjsToVector2D(start_position[1], start_position[2]));

    if(level_data["music"] && utils::Settings::reference()->background_music())
        world->set_background_music(ugdk::audio::manager()->LoadMusic(level_data["music"].value<std::string>()));
}

} // namespace utils
