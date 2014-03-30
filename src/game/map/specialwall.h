#ifndef HORUSEYE_GAME_MAP_SPECIALWALL_H_
#define HORUSEYE_GAME_MAP_SPECIALWALL_H_

#include <ugdk/graphic.h>
#include <memory>

namespace map {

std::shared_ptr<ugdk::graphic::Primitive> CreateSpecialWall(const ugdk::graphic::Texture*);

} // namespace map

#endif /* HORUSEYE_GAME_MAP_GIANTFLOOR_H_ */
