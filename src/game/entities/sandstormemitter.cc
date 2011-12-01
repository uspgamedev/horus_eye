
#include "game/entities/sandstormemitter.h"

#include "game/scenes/world.h"

namespace entities {

using scene::World;

void SandstormEmitter::Update(float dt) {
    if(!suspended_) {
        // emite as bullets
        World *world = WORLD();

        //TODO
    }
}

} // namespace entitites