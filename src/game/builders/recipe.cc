#include "game/builders/recipe.h"

#include "game/sprites/worldobject.h"

namespace builder {

sprite::WorldObject* Recipe::Generate() const {
    sprite::WorldObject *wobj = new sprite::WorldObject;
    for(const std::shared_ptr<Director>& director : directors_)
        director->Visit(wobj);
    return wobj;
}

} // namespace builder
