//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/textmanager.cc
// Implementacao da classe TextManager.
//

#include "engine.h"
#include "textmanager.h"

namespace framework{

bool TextManager::Initialize() {
    TTF_Init();
    textColor_ = { 255, 255, 255 };
    transparentColor_ = { 255, 0, 255 };
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
        if(!img->setSurface(message)) {
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
    
    img->Create(video_size);
    img->Clear(SDL_MapRGB(img->format() , transparentColor_.r,  transparentColor_.g, transparentColor_.b));
    img->setColorKey(transparentColor_);

    //Blit lines in transparent surface
    for(int i = 0; i<nlines; i++) {
        linesurf = TTF_RenderText_Solid( font_, lines[i].c_str(), textColor_ );
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

Image* TextManager::LoadFile(string path){
    return NULL;
}

} // namespace framework
