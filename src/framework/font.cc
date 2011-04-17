#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>
#include "types.h"
#include "font.h"
#include "image.h"

namespace framework {

Font::Font(Image ** letters, int fontsize, char ident, bool fancy) 
	: size_(fontsize), letters_(letters) {

	id_ = glGenLists(256);
	Vector2D blank;
	for(int c = 0; c < 256; c++) {
		glNewList(id_ + c, GL_COMPILE);
			glPushMatrix();
			Vector2D lettersize = letters_[c]->render_size() * (size_ * 0.01f);
			glScalef(lettersize.x, lettersize.y, 1.0f);
			letters_[c]->RawDraw(0);
			glPopMatrix();
			glTranslatef(lettersize.x, 0, 0);
		glEndList();
	}
	switch(ident) {
		case 'l':
			ident_ = LEFT;
			break;
		case 'r':
			ident_ = RIGHT;
			break;
		case 'c':
		default:
			ident_ = CENTER;
			break;
	}
	fancy_ = fancy;
}

Font::~Font() {
}

Vector2D Font::GetLetterSize(unsigned char letter) {
	return letters_[letter]->render_size() * (size_ * 0.01f);
}

}  // namespace framework
