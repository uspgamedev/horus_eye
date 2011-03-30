#ifndef HORUSEYE_FRAMEWORK_TEXT_H_
#define HORUSEYE_FRAMEWORK_TEXT_H_

#include <string>
#include "vector2D.h"
#include "drawable.h"

namespace framework {

class Text : public Drawable {
  public:
	Text(std::string message, int font);
	~Text() {}

	bool DrawTo(const Vector2D& position, int frame_number, uint8 mirror, 
		const Color& color, float alpha, const Vector2D& draw_size);

	virtual Vector2D render_size() const;
	int width();
	int height();

  private:
	int font_, width_, height_;
	std::string message_;
};

}  // namespace framework

#endif
