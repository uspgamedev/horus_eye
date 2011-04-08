#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>
#include "types.h"
#include "font.h"
#include "image.h"

namespace framework {

Font::Font(string path, int fontsize, char ident, bool fancy) {
	TTF_Font *ttf_font = TTF_OpenFont( path.c_str(), fontsize );
	char str[2];
	int c;
	id_ = glGenLists(256);
	letters_ = new Image*[256];
	Vector2D blank;
	Color color = Image::CreateColor(1.0f, 1.0f, 1.0f);
	SDL_Color sdlcolor = { 255, 255, 255 };
	str[1] = '\0';
	for(c = 1; c < 256; c++) {
		str[0] = (char)(c);
		SDL_Surface *letter = TTF_RenderUTF8_Blended( ttf_font, str, sdlcolor );
		letters_[c] = new Image;
		letters_[c]->LoadFromSurface(letter);
		SDL_FreeSurface(letter);
		glNewList(id_ + c, GL_COMPILE);
			letters_[c]->DrawTo(blank, 0, 0, color, 1.0f, letters_[c]->render_size());
			glTranslatef(letters_[c]->render_size().x, 0, 0);
		glEndList();
	}
	TTF_CloseFont( ttf_font );
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
	return letters_[letter]->render_size();
}

}  // namespace framework
