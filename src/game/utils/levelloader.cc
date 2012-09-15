#include "levelloader.h"

#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>

#include "game/scenes/world.h"
#include "game/map/loader.h"
#include "game/utils/settings.h"

namespace utils {

using ugdk::script::VirtualObj;
using ugdk::math::Integer2D;

void LevelLoader::Load(const std::string& name) {
    VirtualObj level_data = SCRIPT_MANAGER()->LoadModule("levels." + name);
    if(!level_data) return;

    int width = level_data["width"].value<int>();
    int height = level_data["height"].value<int>();
    world_->set_size(Integer2D(width, height));

    world_->SetupCollisionManager();

    map::Room* room = map::LoadRoom(level_data["room"].value<std::string>());
    world_->SetRoom(room);

    if(level_data["music"] && utils::Settings::reference()->background_music())
        world_->set_background_music(AUDIO_MANAGER()->LoadMusic(level_data["music"].value<std::string>()));
}

} // namespace utils
