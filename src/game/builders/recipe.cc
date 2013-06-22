#include "game/builders/recipe.h"

#include "game/sprites/worldobject.h"

namespace builder {

sprite::WorldObject* Recipe::Generate() const {
    sprite::WorldObject *wobj = new sprite::WorldObject;
    for(const Director& director : directors_)
        director(wobj);
    return wobj;
}

} // namespace builder
