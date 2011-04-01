#include "text.h"
#include "engine.h"
#include "textmanager.h"
#include <SDL/SDL_opengl.h>

namespace framework {

Text::Text(std::string message, int font) : font_(font) {
	int i;
	message_.push_back(message);
	width_ = 0;
	line_size_ = height_ = TEXT_MANAGER()->GetLetterSize(message[0]).y;
	for(i = 0; i < message.length(); i++)
		width_ += TEXT_MANAGER()->GetLetterSize(message[i]).x;
}

Text::Text(std::vector<std::string> message, int font) : font_(font) {
	int i, j, width, size;
	width_ = 0;
	line_size_ = 0;
	for(j = message.size() -1; j >= 0; j--) {
		width = 0;
		size = 0;
		message_.push_back(message[j]);
		height_ += (size = TEXT_MANAGER()->GetLetterSize(message[j][0]).y);
		for(i = 0; i < message[j].length(); i++)
			width += TEXT_MANAGER()->GetLetterSize(message[j][i]).x;
		width_ = std::max(width, width_);
		line_size_ = std::max(size, line_size_);
	}
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
	for(int i = 0; i < message_.size(); ++i) {
		glPushMatrix();
		glCallLists(message_[i].length(), GL_UNSIGNED_BYTE, (GLubyte *) message_[i].c_str());
		glPopMatrix();
		glTranslatef( 0, line_size_, 0);
	}
	glPopMatrix();
	return true;
}

}  // namespace framework
