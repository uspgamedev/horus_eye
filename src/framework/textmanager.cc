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


// TODO: implement this as flag on Text
static Image* LoadFancyLine(string line) {

        TTF_Font *font_;
        SDL_Color textColor_;
        SDL_Color transparentColor_;
	
    SDL_Surface *message = NULL;
    SDL_Surface *message_dark = NULL;
    SDL_Surface *message_light = NULL;
    Vector2D size;
    SDL_Color color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    SDL_Rect rect;

    message = TTF_RenderUTF8_Solid( font_, line.c_str(), color );
    color.r = 212;
    color.g = 170;
    color.b = 0;
    message_light = TTF_RenderUTF8_Solid( font_, line.c_str(), color );
    color.r = 85;
    color.g = 68;
    color.b = 0;
    message_dark = TTF_RenderUTF8_Solid( font_, line.c_str(), color );

    rect.x = 2;
    rect.y = 6;
    size.x = message->w;
    size.y = TTF_FontLineSkip(font_)+12;

    SDL_Surface *temp_surface = Image::CreateSurface(size);
    SDL_Surface *surface = SDL_DisplayFormatAlpha(temp_surface);
    SDL_FreeSurface(temp_surface);
    Uint32 transparentColor = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, transparentColor);

    SDL_BlitSurface(message_light, NULL, surface, &rect);
    rect.x -= 1.0;
    rect.y -= 1.0;
    SDL_BlitSurface(message_dark, NULL, surface, &rect);
    rect.x -= 1.0;
    rect.y -= 1.0;
    SDL_BlitSurface(message, NULL, surface, &rect);

    Image *img = new Image;
	if(!img->LoadFromSurface(surface)) {
		delete img;
		img = NULL;
	}
    SDL_FreeSurface(surface);
    return img;
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
	fonts_[name] = current_font_ = new Font(path, size, ident, fancy);
}


} // namespace framework