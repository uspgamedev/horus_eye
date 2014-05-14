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

void LoadCollisionClasses(const VirtualObj& classes_vector, pyramidworks::collision::CollisionManager* collision_manager) {
    if (classes_vector) {
        for (VirtualObj& it : classes_vector.value<VirtualObj::Vector>()) {
            VirtualObj::Vector&& collclass = it.value<VirtualObj::Vector>();
            if (collclass.size() >= 2)
                collision_manager->ChangeClassParent(collclass[0].value<std::string>(), collclass[1].value<std::string>());
        }
    }
}

void LoadLevel(const VirtualObj& level_data, const std::string& level_path, scene::World** world_ptr) {
    *world_ptr = nullptr;
    SCRIPT_MANAGER()->LoadModule("event") ["ClearAll"] ();
    if(!level_data) return;

    if(!level_data["width"] || !level_data["height"] || !level_data["rooms"] || !level_data["new"]) 
        return;

    auto level_vobj = level_data["new"]();

    int width = level_data["width"].value<int>();
    int height = level_data["height"].value<int>();

    scene::World* world = *world_ptr = new scene::World(Integer2D(width, height), level_vobj);

    LoadCollisionClasses(level_data["collision_classes"], world->collision_manager());

    for (VirtualObj& it : level_data["rooms"].value<VirtualObj::List>()) {
        VirtualObj::Vector&& room_data = it.value<VirtualObj::Vector>();
        if(room_data.size() != 3) continue;

        int x = room_data[0].value<int>();
        int y = room_data[1].value<int>();
        std::string room_name = room_data[2].value<std::string>();

        VirtualObj room_vobj = level_data[room_name].valid()
            ? level_data[room_name]
            : SCRIPT_MANAGER()->LoadModule(level_path + "." + room_name);

        if (map::IsValidRoomData(room_vobj))
            LoadCollisionClasses(room_vobj["collision_classes"], world->collision_manager());

        map::Room* room = map::LoadRoom(room_name, room_vobj, { x, y });
        if(room) {
            world->AddRoom(room);
        } else {
            printf("Room '%s' could not be loaded.\n", room_name.c_str());
        }
    }

    if(level_data["music"] && utils::Settings::reference()->background_music())
        world->set_background_music(ugdk::audio::manager()->LoadMusic(level_data["music"].value<std::string>()));
}

} // namespace utils
