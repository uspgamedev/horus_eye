#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <clocale>
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
	setlocale (LC_ALL,"en_US.C-UTF-8");
    return true;
}

bool TextManager::Destroy() {
	TTF_Quit();
	return true;
}

Text* TextManager::GetText(wstring text, wstring fonttag, int width) {
	wstring subString;
    vector<wstring> lines;
	Font *font = fonttag.size() > 0 ? fonts_[fonttag] : current_font_;
	int screensize = ((width == -1) ? static_cast<int>(VIDEO_MANAGER()->video_size().x) : width) - 200;
	int cur_width = 0, last_break = 0;
	for(unsigned int i = 0; i < text.length(); i++) {
		if(text[i] == L'\n' || (text[i] == L' ' && cur_width > screensize)) {
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
Text* TextManager::GetTextFromFile(wstring path, wstring font, int width) {
	std::string fullpath = PATH_MANAGER()->ResolvePath(path);
	FILE *txtFile = fopen(fullpath.c_str(), "r,ccs=UTF-8");
	if(txtFile==NULL) return NULL;
	wchar_t buffer[MAXLINE];
	wstring line, output;
    while(fgetws(buffer, 200, txtFile)!=NULL){
        line=buffer;
        output.append(line);
    }
	return GetText(output, font, width);
}

//static std::wstring characters_table = 
// L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789çàáéíóúâêôãõü,. !?:;/()[]{}%'\"";
void TextManager::AddFont(wstring name, wstring path, int size, char ident, bool fancy) {
	if(fonts_.count(name) > 0)
		return;
	Image **font_image = NULL;
	fprintf(stderr, "Loading new font tag: \"%s\"\n", name.c_str());
	if(font_images_.count(path) == 0) {
		// Given file not loaded, loading it.
		font_image = new Image*[65535];
		memset(font_image, NULL, 4*65535);
		TTF_Font *ttf_font = TTF_OpenFont( PATH_MANAGER()->ResolvePath(path).c_str(), 100 );
		fwprintf(stderr, L"-- Processing new font file: \"%s\"\n", path.c_str());
		SDL_Color sdlcolor = { 255, 255, 255 };
		uint16 str[2];
		str[1] = '\0'; // End of string
		unsigned int i = 0x20;
		while(i < 256) {
			// For each possible character in the extended ASCII table, render and store it.
			// Could be improved to ignore non-renderable characters, like linefeed.
			str[0] = (uint16)(i);
			fwprintf(stderr, L"\t(%u) \"%c\": ", i, str[0]);
			SDL_Surface *letter = TTF_RenderUNICODE_Blended( ttf_font, str, sdlcolor );
			font_image[i] = new Image;
			font_image[i]->LoadFromSurface(letter);
			SDL_FreeSurface(letter);

			i++;
			if(i == 128)
				i = 160;
		}
		TTF_CloseFont( ttf_font );
		font_images_[path] = font_image;
	} else
		font_image = font_images_[path];
	fonts_[name] = current_font_ = new Font(font_image, size, ident, fancy);
}


} // namespace framework