#ifndef HORUSEYE_GAME_MAP_GIANTFLOOR_H_
#define HORUSEYE_GAME_MAP_GIANTFLOOR_H_

#include <ugdk/internal.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/math/integer2D.h>

namespace map {

class GiantFloor : public ugdk::graphic::Drawable {
  public:
    GiantFloor(const ugdk::math::Integer2D& size);
    ~GiantFloor();

    void Draw(ugdk::graphic::Canvas&) const override;
    const ugdk::math::Vector2D& size() const { return size_; }

  private:
    ugdk::math::Vector2D size_;
    ugdk::internal::GLTexture* texture_;
    ugdk::graphic::opengl::VertexBuffer* vertexbuffer_;
    ugdk::graphic::opengl::VertexBuffer* uvbuffer_;
};
} // namespace map

#endif /* HORUSEYE_GAME_MAP_GIANTFLOOR_H_ */
