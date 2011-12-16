#ifndef HORUSEYE_FRAMEWORK_LIGHT_H_
#define HORUSEYE_FRAMEWORK_LIGHT_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>

namespace ugdk {

class Light {
  public:
    Light();
	~Light() {}

    Vector2D hotspot() const { return hotspot_; }
    void set_hotspot(const Vector2D& hotspot) { hotspot_ = hotspot; }

	Vector2D dimension() const { return dimension_; }
	void set_dimension(Vector2D dimension) { dimension_ = dimension; }

    Color color() const { return color_; }
    void set_color(const Color& color) { color_ = color; }

	
	void Render(Vector2D &offset);

  private:
	Vector2D dimension_, hotspot_;
    Color color_;
};

}  // namespace framework

#endif
