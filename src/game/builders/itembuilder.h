#ifndef HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_

#include <vector>
#include <string>
#include <ugdk/graphic.h>
#include "game/sprites.h"

namespace builder {
namespace ItemBuilder {

sprite::WorldObject* LifePotion(const std::vector<std::string>& arguments);
sprite::WorldObject* ManaPotion(const std::vector<std::string>& arguments);
sprite::WorldObject* SightPotion(const std::vector<std::string>& arguments);
sprite::WorldObject* BlueGem(const std::vector<std::string>& arguments);

}
}

#endif /* HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_ */
