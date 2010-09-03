//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/textmanager.h
// Definicao da classe TextManager.
//

#ifndef HORUSEYE_FRAMEWORK_TEXTMANAGER_H_
#define HORUSEYE_FRAMEWORK_TEXTMANAGER_H_

#include <vector>
#include <SDL/SDL_ttf.h>
#include "image.h"
#include "vector2D.h"
#include "videomanager.h"
using namespace std;

#define TEXT_MANAGER() framework::Engine::reference()->text_manager()

namespace framework {

class TextManager{
    public:
        TextManager() : font_(NULL) {}
        ~TextManager() {}

        bool Initialize();
        bool Destroy();
        bool setFont(string font, int fontsize, string *style);
        bool setColor(int r, int g, int b);
        TTF_Font* getFont();

        Image* LoadLine(string line);
        Image* LoadText(string text, char indent);
        Image* LoadFile(string path);

    private:
        TTF_Font *font_;
        SDL_Color textColor_;
        SDL_Color transparentColor_;
};

} // namespace framework

#endif
