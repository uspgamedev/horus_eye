
#include <cstdio>
#include "game/context.h"
#include "game/scenes/world.h"
#include "game/utils/levelmanager.h"

namespace context {

using std::string;
using sprite::WorldObject;
using scene::World;

WorldObject* WorldObjectByTag (const std::string& tag) {
    return utils::LevelManager::reference()->get_current_level()
            ->WorldObjectByTag(tag);
}

void CreateTag (WorldObject* obj, const std::string& tag) {
    World* world = utils::LevelManager::reference()->get_current_level();
    if (world)
        world->CreateTag(obj, tag);
    else
        fprintf(stderr, "Attempt to create tag in NULL world.\n");
}

} // namespace context
