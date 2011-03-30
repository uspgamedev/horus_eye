#ifndef HORUSEYE_FRAMEWORK_DRAWABLE_H_
#define HORUSEYE_FRAMEWORK_DRAWABLE_H_

#include "vector2D.h"
#include "types.h"

namespace framework {

class Drawable {
  public:
	~Drawable() {}
	virtual bool DrawTo(const Vector2D& position, int frame_number, uint8 mirror, 
		const Color& color, float alpha, const Vector2D& draw_size) {
			return false;
	}

  protected:
	Drawable() {}
};

}  // namespace framework

#endif
