//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/textmanager.cc
// Implementacao da classe TextManager.
//

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
    font_ = TTF_OpenFont( "data/font/Filmcryptic.ttf", 28 );
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
    
    message = TTF_RenderText_Solid( font_, line.c_str(), textColor_ );
    
    if(img != NULL) {
        if(!img->SetSurface(message)) {
            delete img;
            return NULL;
        }
//        memory_[text] = img;
    }
    else
        return NULL;

    return img;
//    return memory_[text];
}
        
Image* TextManager::LoadText(string text, char indent) {
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
    video_size.y = nlines*lineskip;
    
    SDL_Surface *surface = Image::CreateSurface(video_size);
    SDL_Rect fillRect = {0, 0, surface->w, surface->h};

    Uint32 transparentColor = SDL_MapRGB(surface->format,
            transparentColor_.r,  transparentColor_.g, transparentColor_.b);
    SDL_FillRect(surface, &fillRect, transparentColor);
    SDL_SetColorKey(surface, SDL_SRCCOLORKEY, transparentColor);

    //Blit lines in transparent surface
    for(int i = 0; i<nlines; i++) {
        linesurf = TTF_RenderText_Solid( font_, lines[i].c_str(), textColor_ );
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

    img->SetSurface(surface);
    SDL_FreeSurface(surface);
    return img;
}

Image* TextManager::LoadFile(string path, char indent){
    FILE* txtFile;
    char buffer[MAXLINE];
    int fontwidth = 24;
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
