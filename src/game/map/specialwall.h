#ifndef HORUSEYE_GAME_MAP_SPECIALWALL_H_
#define HORUSEYE_GAME_MAP_SPECIALWALL_H_

#include <ugdk/graphic.h>
#include <string>

namespace map {

void PreparePrimitiveSpecialWall(ugdk::graphic::Primitive&, const ugdk::graphic::TextureAtlas*, const std::string& frame_name);

} // namespace map

#endif /* HORUSEYE_GAME_MAP_SPECIALWALL_H_ */
