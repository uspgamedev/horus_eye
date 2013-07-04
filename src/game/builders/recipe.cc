#include "game/builders/recipe.h"

#include "game/sprites/worldobject.h"

namespace builder {

sprite::WorldObject* Recipe::Generate() const {
    sprite::WorldObject *wobj = new sprite::WorldObject;
    for(const Property& property : properties_)
        property(wobj);
    return wobj;
}

} // namespace builder
