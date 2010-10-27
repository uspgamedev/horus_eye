#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "engine.h"
#include "textmanager.h"

using namespace std;

//Tamanho máximo de uma linha do arquivo de entrada
#define MAXLINE 200

namespace framework{

bool TextManager::Initialize() {
    TTF_Init();
    textColor_.r = 255;
    textColor_.g = 255;
    textColor_.b = 255;
    transparentColor_.r = 255;
    transparentColor_.g = 0;
    transparentColor_.b = 255;
    font_ = TTF_OpenFont( "data/font/Filmcrypob.ttf", 60 );
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
    if(style != NULL){

    }
    return true;
}

TTF_Font* TextManager::getFont(){
    return font_;
}

Image* TextManager::LoadLine(string line) {
    Image* img = new Image;
    SDL_Surface *message = NULL;
    
    message = TTF_RenderUTF8_Blended( font_, line.c_str(), textColor_ );
    
    if(img != NULL) {
        if(!img->setSurface(message)) {
            delete img;
            return NULL;
        }
    }
    else
        return NULL;

    return img;
}

Image* TextManager::LoadFancyLine(string line) {
    Image* img = new Image;
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

    img->Create(size);
    img->Clear(SDL_MapRGB(img->format() , transparentColor_.r,  transparentColor_.g, transparentColor_.b));
    img->setColorKey(transparentColor_);

    img->blitSurface(message_light, NULL, &rect);
    rect.x -= 1.0;
    rect.y -= 1.0;
    img->blitSurface(message_dark, NULL, &rect);
    rect.x -= 1.0;
    rect.y -= 1.0;
    img->blitSurface(message, NULL, &rect);

    return img;
}
        
Image* TextManager::LoadText(string text, char indent, float width = -1) {
    Image *img = new Image;
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
    
    img->Create(video_size);
    img->Clear(SDL_MapRGB(img->format() , transparentColor_.r,  transparentColor_.g, transparentColor_.b));
    img->setColorKey(transparentColor_);

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
        img->blitSurface(linesurf, NULL, &rect);
    }

    return img;
}

Image* TextManager::LoadFile(string path, char indent) {
    FILE* txtFile;
    char buffer[MAXLINE];
    int fontwidth = 32;
    int screensize = 1024;
    int nchar=0, pos=0, last=0;
    string line, output;

    txtFile = fopen(path.c_str(), "r");
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

} // namespace framework
