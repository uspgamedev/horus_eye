#ifndef HORUSEYE_GAME_MAP_GIANTFLOOR_H_
#define HORUSEYE_GAME_MAP_GIANTFLOOR_H_

#include <ugdk/internal.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/math/integer2D.h>

#include "game/map.h"

namespace map {

class GiantFloor {
  public:
    GiantFloor(const Room* room);
    ~GiantFloor();

    void set_hotspot(const ugdk::math::Vector2D& hotspot) { hotspot_ = hotspot; }
    void Draw(ugdk::graphic::Canvas&, const ugdk::graphic::TextureUnit& light_unit) const;

  private:
    const Room* room_;
    ugdk::math::Vector2D size_;
    ugdk::math::Vector2D hotspot_;
    ugdk::graphic::VertexData data_;
    ugdk::internal::GLTexture* texture_;
};
} // namespace map

#endif /* HORUSEYE_GAME_MAP_GIANTFLOOR_H_ */
