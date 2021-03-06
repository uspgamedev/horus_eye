#ifndef HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_

#include <vector>
#include <string>
#include "game/sprites.h"

namespace builder {
namespace ItemBuilder {

sprite::WObjPtr LifePotion(const std::vector<std::string>& arguments);
sprite::WObjPtr ManaPotion(const std::vector<std::string>& arguments);

}
}

#endif /* HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_ */
