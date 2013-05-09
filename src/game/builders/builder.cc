#include "game/builders/builder.h"

#include <unordered_map>
#include <functional>

#include "game/map/tile.h"
#include "game/builders/doodadbuilder.h"
#include "game/builders/mummybuilder.h"
#include "game/builders/itembuilder.h"
#include "game/builders/scriptbuilder.h"

namespace builder {

using std::string;
using std::unordered_map;

typedef std::function<sprite::WorldObject* (const ArgumentList&)> WorldObjectFactoryMethod;
typedef unordered_map<string, WorldObjectFactoryMethod> WorldObjectFactoryMap;

static WorldObjectFactoryMap build_type_factory_map() {
    WorldObjectFactoryMap m;
    m[string(1, STANDING_MUMMY)] = builder::MummyBuilder::StandingMummy;
    m[string(1, STANDING_BIG_MUMMY)] = builder::MummyBuilder::StandingBigMummy;
    m[string(1, STANDING_RANGED_MUMMY)] = builder::MummyBuilder::StandingRangedMummy;
    m[string(1, STANDING_PAPER_MUMMY)] = builder::MummyBuilder::StandingPaperMummy;
    m[string(1, STANDING_PHARAOH)] = builder::MummyBuilder::StandingPharaoh;
    m[string(1, MUMMY)] = builder::MummyBuilder::WalkingMummy;
    m[string(1, BIG_MUMMY)] = builder::MummyBuilder::WalkingBigMummy;
    m[string(1, RANGED_MUMMY)] = builder::MummyBuilder::WalkingRangedMummy;
    m[string(1, PHARAOH)] = builder::MummyBuilder::StandingPharaoh;
    m[string(1, POTIONL)] = builder::ItemBuilder::LifePotion;
    m[string(1, POTIONM)] = builder::ItemBuilder::ManaPotion;
    m[string(1, POTIONS)] = builder::ItemBuilder::SightPotion;
    m[string(1, BLUEGEM)] = builder::ItemBuilder::BlueGem;
    m[string(1, SCRIPT)] = (sprite::WorldObject* (*)(const ArgumentList&)) builder::ScriptBuilder::Script;
    m[string(1, DOOR)] = builder::DoodadBuilder::Door;
    m[string(1, WALL)] = builder::DoodadBuilder::Wall;
    m[string(1, ENTRY)] = builder::DoodadBuilder::Entry;
    return m;
}

static WorldObjectFactoryMap type_factory_map = build_type_factory_map();

sprite::WorldObject* WorldObjectFromTypename(const string& type, const ArgumentList& arguments) {
    auto iterator = type_factory_map.find(type);
    if(iterator != type_factory_map.end())
        return iterator->second(arguments);
    else
        return NULL;
}

bool HasFactoryMethod(const std::string& type) {
    return type_factory_map.find(type) != type_factory_map.end();
}

} // namespace builder
