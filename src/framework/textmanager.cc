#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <SDL/SDL_opengl.h>
#include "engine.h"
#include "textmanager.h"
#include "pathmanager.h"
#include "text.h"

using namespace std;

//Tamanho máximo de uma linha do arquivo de entrada
#define MAXLINE 1024

namespace framework{

bool TextManager::Initialize() {
    TTF_Init();
    textColor_.r = 255;
    textColor_.g = 255;
    textColor_.b = 255;
    font_ = TTF_OpenFont( "data/font/Filmcrypob.ttf", 60 );
    transparentColor_.r = 255;
    transparentColor_.g = 255;
    transparentColor_.b = 0;

	LoadFont("data/font/Filmcrypob.ttf", 60);
	current_font_ = fonts_["data/font/Filmcrypob.ttf"];
    return true;
}

bool TextManager::Destroy() {
   if(font_ != NULL)
       TTF_CloseFont( font_ );
   TTF_Quit();
   return true;
}

bool TextManager::setFont(string font, int fontsize, string *style) {
    font_ = TTF_OpenFont( font.c_str(), fontsize );
	LoadFont(font.c_str(), fontsize);
	current_font_ = fonts_[font.c_str()];
    if(style != NULL){

    }
    return true;
}

TTF_Font* TextManager::getFont(){
    return font_;
}

Image* TextManager::LoadLine(string line) {
    SDL_Surface *message = NULL;
    
    message = TTF_RenderUTF8_Blended( font_, line.c_str(), textColor_ );
    Image *img = new Image;
    bool result = img->LoadFromSurface(message);
    SDL_FreeSurface(message);

	if(!result) {
        delete img;
		img = NULL;
	}
    return img;
}

Image* TextManager::LoadFancyLine(string line) {
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
        
Image* TextManager::LoadText(string text, char indent, float width = -1) {
    Image *img = NULL;
    string subString, temp(text);
    vector<string> lines;
    int lineskip = TTF_FontLineSkip(font_);
    int nlines=0, n=0;
    SDL_Surface* linesurf = NULL;
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;

    //Split lines
    n = temp.find("\n");
    while(n!=-1) {
        subString = temp.substr(0, n);
        lines.push_back(subString);
        temp = temp.substr(n+1, string::npos);
        n = temp.find("\n");
        nlines++;
    }
    
    //Surface transparent
    Vector2D video_size = VIDEO_MANAGER()->video_size();
    if(width>0)
        video_size.x = width;
    video_size.y = nlines*lineskip;

    SDL_Surface *temp_surface = Image::CreateSurface(video_size);
    SDL_Surface *surface = SDL_DisplayFormatAlpha(temp_surface);
    SDL_FreeSurface(temp_surface);
    SDL_Rect fillRect = {0, 0, surface->w, surface->h};
    Uint32 transparentColor = SDL_MapRGBA(surface->format, 0, 0, 0, 255);
    SDL_FillRect(surface, &fillRect, transparentColor);

    //Blit lines in transparent surface
    for(int i = 0; i<nlines; i++) {
        linesurf = TTF_RenderUTF8_Solid( font_, lines[i].c_str(), textColor_ );
        if(linesurf==NULL) continue;
        switch (indent){
            case 'c':
                rect.x = (video_size.x - linesurf->w)/2;
                break;
            case 'l':
                rect.x = 0;
                break;
            case 'r':
                rect.x = video_size.x - linesurf->w;
                break;
            default:
                rect.x = 0;
        }
        rect.y = i*lineskip;    
        SDL_BlitSurface(linesurf, NULL, surface, &rect);
    }

	img = new Image;
	if(!img->LoadFromSurface(surface)) {
        delete img;
		img = NULL;
	}
    SDL_FreeSurface(surface);
    return img;
}

Image* TextManager::LoadFile(string path, char indent) {
    FILE* txtFile;
    char buffer[MAXLINE];
    int fontwidth = 32;
    int screensize = 1024;
    int nchar=0, pos=0, last=0;
    string line, output;

	std::string fullpath = PATH_MANAGER()->ResolvePath(path);

    txtFile = fopen(fullpath.c_str(), "r");
    if(txtFile==NULL) return NULL;

    nchar = screensize/fontwidth;

    while(fgets(buffer, 200, txtFile)!=NULL){
        line=buffer;
        while(pos!=-1){
            pos = line.find(" ", last+1);
            if(pos >= nchar){
                output.append(line.substr(0, last));
                output.append("\n");
                line = line.substr(last+1, string::npos);
                last=0;
            }
            else{
                last = pos;
            }
        }
        pos = 0;
        output.append(line);
    }

    return LoadText(output, indent);
}

Text* TextManager::GetText(string text) {
	return new Text(text, current_font_.id);
}

Vector2D TextManager::GetLetterSize(char letter) {
	return current_font_.letters[letter]->render_size();
}

void TextManager::LoadFont(string path, int fontsize) {
	if(fonts_.count(path) > 0)
		return;

	TTF_Font *ttf_font = TTF_OpenFont( path.c_str(), fontsize );
	char str[2];
	int c;
	Font font;
	font.id = glGenLists(256);
	font.letters = new Image*[256];
	Vector2D blank;
	Color color = Image::CreateColor(1.0f, 1.0f, 1.0f);
	str[1] = '\0';
	for(c = 1; c < 256; c++) {
		str[0] = (char)(c);
		SDL_Surface *letter = TTF_RenderUTF8_Blended( ttf_font, str, textColor_ );
		font.letters[c] = new Image;
		font.letters[c]->LoadFromSurface(letter);
		SDL_FreeSurface(letter);
		glNewList(font.id + c, GL_COMPILE);
			font.letters[c]->DrawTo(blank, 0, 0, color, 1.0f, font.letters[c]->render_size());
			glTranslatef(font.letters[c]->render_size().x, 0, 0);
		glEndList();
	}
	fonts_[path] = font;
}


} // namespace framework