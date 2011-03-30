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

	virtual Vector2D render_size() const { return Vector2D(); }

	// DEPRECATED DO NOT USE OR DIE LOLOL
	virtual int width() const { return (int)(render_size().x); }
	virtual int height() const { return (int)(render_size().y); }
	virtual bool Destroy() { return false; }

  protected:
	Drawable() {}
};

}  // namespace framework

#endif
