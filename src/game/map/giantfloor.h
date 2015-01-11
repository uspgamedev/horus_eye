#ifndef HORUSEYE_GAME_MAP_GIANTFLOOR_H_
#define HORUSEYE_GAME_MAP_GIANTFLOOR_H_

#include <ugdk/graphic.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/math/integer2D.h>

#include "game/map.h"
#include "frontend/gameview.h"

namespace map {

class GiantFloor {
  public:
    GiantFloor(const Room* room);
    ~GiantFloor();

    void Draw(ugdk::graphic::Canvas&,
              const ugdk::graphic::TextureUnit& light_unit,
              const frontend::gameview::LightRendering&) const;

  private:
    const Room* room_;
    ugdk::graphic::VertexData data_;
    ugdk::graphic::GLTexture* texture_;
};
} // namespace map

#endif /* HORUSEYE_GAME_MAP_GIANTFLOOR_H_ */
