#include "explosionbuilder.h"

#include "game/builders/builder.h"
#include "game/builders/recipe.h"

namespace builder {
namespace ExplosionBuilder {

sprite::WorldObject* FireballExplosion() {
    return FindRecipeFor("fireball_explosion")->Generate();
}

} // namespace ExplosionBuilder
} // namespace builder
