#ifndef HORUSEYE_GAME_MAP_SPECIALWALL_H_
#define HORUSEYE_GAME_MAP_SPECIALWALL_H_

#include <ugdk/graphic/drawable.h>
#include <ugdk/math/integer2D.h>

namespace map {

class SpecialWall : public ugdk::graphic::Drawable {
  public:
    SpecialWall(const ugdk::graphic::Texture*);
    ~SpecialWall();

    void Draw(const ugdk::graphic::Geometry&, const ugdk::graphic::VisualEffect&) const;
    const ugdk::math::Vector2D& size() const { return size_; }

  private:
    static ugdk::graphic::opengl::ShaderProgram* wall_light_shader_;

    ugdk::math::Vector2D size_;
    const ugdk::graphic::Texture* texture_;
};
} // namespace map

#endif /* HORUSEYE_GAME_MAP_GIANTFLOOR_H_ */
