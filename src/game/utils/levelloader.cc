#include "levelloader.h"

#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>

#include "game/scenes/world.h"
#include "game/map/loader.h"
#include "game/map/room.h"
#include "game/utils/settings.h"

namespace utils {

using ugdk::script::VirtualObj;
using ugdk::math::Integer2D;

void LevelLoader::Load(const std::string& name) {
    VirtualObj level_data = SCRIPT_MANAGER()->LoadModule("levels." + name);
    if(!level_data) return;

    if(!level_data["width"] || !level_data["height"] || !level_data["rooms"]) return;

    int width = level_data["width"].value<int>();
    int height = level_data["height"].value<int>();
    world_->set_size(Integer2D(width, height));

    world_->SetupCollisionManager();

    VirtualObj::List rooms = level_data["rooms"].value<VirtualObj::List>();
    for(VirtualObj::List::iterator it = rooms.begin(); it != rooms.end(); ++it) {
        VirtualObj::Vector room_data = it->value<VirtualObj::Vector>();
        if(room_data.size() != 3) continue;
        int x = room_data[0].value<int>();
        int y = room_data[1].value<int>();
        map::Room* room = map::LoadRoom(room_data[2].value<std::string>());
        room->set_offset(Integer2D(x, y));
        world_->SetRoom(room);
    }

    if(level_data["music"] && utils::Settings::reference()->background_music())
        world_->set_background_music(AUDIO_MANAGER()->LoadMusic(level_data["music"].value<std::string>()));
}

} // namespace utils
