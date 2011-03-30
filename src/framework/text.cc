#include "text.h"
#include "engine.h"
#include "textmanager.h"
#include <SDL/SDL_opengl.h>

namespace framework {

Text::Text(std::string message, int font) : font_(font), message_(message) {
	int i;
	width_ = 0;
	height_ = TEXT_MANAGER()->GetLetterSize(message[0]).y;
	for(i = 0; i < message.length(); i++)
		width_ += TEXT_MANAGER()->GetLetterSize(message[i]).x;
}

Vector2D Text::render_size() const {
	return Vector2D(width_, height_);
}

int Text::width() {
	return 0;
}

int Text::height() {
	return 0;
}

bool Text::DrawTo(const Vector2D& position, int frame_number, uint8 mirror, 
				  const Color& color, float alpha, const Vector2D& draw_size) {

	glPushMatrix();
	glTranslatef( position.x, position.y, 0 );
	glListBase(font_);
	glCallLists(message_.length(), GL_UNSIGNED_BYTE, (GLubyte *) message_.c_str());
	glPopMatrix();
	return true;
}

}  // namespace framework
