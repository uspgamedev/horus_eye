#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <SDL/SDL_opengl.h>
#include "engine.h"
#include "textmanager.h"
#include "pathmanager.h"
#include "videomanager.h"
#include "text.h"
#include "font.h"

using namespace std;

//Tamanho máximo de uma linha do arquivo de entrada
#define MAXLINE 1024

namespace framework{

bool TextManager::Initialize() {
	TTF_Init();
    return true;
}

bool TextManager::Destroy() {
	TTF_Quit();
	return true;
}

Text* TextManager::GetText(string text, string fonttag, int width) {
	string subString;
    vector<string> lines;
	Font *font = fonttag.size() > 0 ? fonts_[fonttag] : current_font_;
	int screensize = ((width == -1) ? static_cast<int>(VIDEO_MANAGER()->video_size().x) : width) - 200;
	int cur_width = 0, last_break = 0;
	for(int i = 0; i < text.length(); i++) {
		if(text[i] == '\n' || (text[i] == ' ' && cur_width > screensize)) {
			subString = text.substr(last_break, i - last_break);
			lines.push_back(subString);
			last_break = i + 1;
			cur_width = 0;
		} else {
			cur_width += static_cast<int>(font->GetLetterSize(text[i]).x);
		}
	}
	if(cur_width > 0) {
		subString = text.substr(last_break, text.length());
		lines.push_back(subString);
	}

	return new Text(lines, font);
}
Text* TextManager::GetTextFromFile(string path, string font, int width) {
	std::string fullpath = PATH_MANAGER()->ResolvePath(path);
	FILE* txtFile = fopen(fullpath.c_str(), "r");
	if(txtFile==NULL) return NULL;
	char buffer[MAXLINE];
	string line, output;
    while(fgets(buffer, 200, txtFile)!=NULL){
        line=buffer;
        output.append(line);
    }
	return GetText(output, font, width);
}

void TextManager::AddFont(string name, string path, int size, char ident, bool fancy) {
	if(fonts_.count(name) > 0)
		return;
	Image **font_image = NULL;
	fprintf(stderr, "Loading new font tag: \"%s\"\n", name.c_str());
	if(font_images_.count(path) == 0) {
		// Given file not loaded, loading it.
		font_image = new Image*[256];
		TTF_Font *ttf_font = TTF_OpenFont( PATH_MANAGER()->ResolvePath(path).c_str(), 100 );
		fprintf(stderr, "-- Processing new font file: \"%s\"\n", path.c_str());
		SDL_Color sdlcolor = { 255, 255, 255 };
		char str[2];
		str[1] = '\0'; // End of string
		for(unsigned int c = 0; c < 256; c++) {
			// For each possible character in the extended ASCII table, render and store it.
			// Could be improved to ignore non-renderable characters, like linefeed.
			str[0] = (char)(c);
			fprintf(stderr, "\t(%u) \"%c\": ", c, str[0]);
			SDL_Surface *letter = TTF_RenderUTF8_Blended( ttf_font, str, sdlcolor );
			font_image[c] = new Image;
			font_image[c]->LoadFromSurface(letter);
			SDL_FreeSurface(letter);
		}
		TTF_CloseFont( ttf_font );
		font_images_[path] = font_image;
	} else
		font_image = font_images_[path];
	fonts_[name] = current_font_ = new Font(font_image, size, ident, fancy);
}


} // namespace framework