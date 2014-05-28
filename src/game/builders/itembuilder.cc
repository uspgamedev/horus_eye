#include "itembuilder.h"

#include "game/builders/scriptbuilder.h"
#include <ugdk/script/virtualobj.h>

namespace builder {
namespace ItemBuilder {

sprite::WObjPtr LifePotion(const std::vector<std::string>& arguments) {
    return ScriptBuilder::Script("potion-life", ugdk::script::VirtualObj());
}

sprite::WObjPtr ManaPotion(const std::vector<std::string>& arguments) {
    return ScriptBuilder::Script("potion-mana", ugdk::script::VirtualObj());
}

} // namespace ItemBuilder
} // namespace builder
