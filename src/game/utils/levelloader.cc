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

void LevelLoader::Load(const std::string& campaign, const std::string& name) {
    VirtualObj level_data = SCRIPT_MANAGER()->LoadModule("campaigns." + campaign + ".levels." + name);
    if(!level_data) return;

    if(!level_data["width"] || !level_data["height"] || !level_data["rooms"] || !level_data["start_position"]) return;

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
        std::string name = room_data[2].value<std::string>();
        map::Room *room = NULL;
        if (level_data[name].valid())
            room = map::LoadRoom(name, level_data[name], Integer2D(x, y));
        else
            room = map::LoadRoom(name, Integer2D(x, y));
        
        if(room) {
            world_->AddRoom(room);
        } else {
            printf("Room '%s' could not be loaded.\n", name.c_str());
        }
    }

    VirtualObj::Vector start_position = level_data["start_position"].value<VirtualObj::Vector>();
    world_->set_hero_initial_data(start_position[0].value<std::string>(), VobjsToVector2D(start_position[1], start_position[2]));

    world_->SetHero(builder::HeroBuilder::Kha());

    if(level_data["music"] && utils::Settings::reference()->background_music())
        world_->set_background_music(ugdk::audio::manager()->LoadMusic(level_data["music"].value<std::string>()));
}

} // namespace utils
