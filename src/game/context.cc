
#include <ugdk/portable/tr1.h>
#include FROM_TR1(unordered_map)

#include "game/context.h"

namespace context {

using std::string;
using std::tr1::unordered_map;
using sprite::WorldObject;

typedef unordered_map<std::string, WorldObject*> TagTable;

static TagTable tagged_;

WorldObject* WorldObjectByTag (const std::string& tag) {
    TagTable::iterator match = tagged_.find(tag);
    if (match == tagged_.end()) return NULL;
    return match->second;
}

void SetTag (WorldObject* obj, const std::string& tag) {
    tagged_[tag] = obj;
}

} // namespace context
