//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/image.cc
// Implementacao da classe Image.
//
#include <SDL/SDL_image.h>
#include "image.h"
#include "frame.h"
#include "videomanager.h"

namespace framework {

// Cria uma imagem com tamanho size.
// Retorna true em caso de sucesso
bool Image::Create(const Vector2D& size) {
    SDL_Surface *screen = SDL_GetVideoSurface();
    int width = static_cast<int>(size.x());
    int height = static_cast<int>(size.y());
    int depth = VideoManager::COLOR_DEPTH;
    Uint32 flags = SDL_HWSURFACE | SDL_SRCCOLORKEY;

    if(screen == NULL)
        return false;

    data_ = SDL_CreateRGBSurface(flags, width, height, depth,
                                 screen->format->Rmask, screen->format->Gmask,
                                 screen->format->Bmask, screen->format->Amask);

    set_frame_size(Vector2D(this->width(), this->height()));
    return (data_ != NULL);
}

// Destroi a imagem
// Retorna true em caso de sucesso
bool Image::Destroy() {
    if (data_ != NULL)
        SDL_FreeSurface(data_);

    return true;
}

// Carrega imagem de um arquivo
// Retorna true em caso de sucesso
bool Image::LoadFromFile(const string& file) {
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimizedImage = NULL;

    loadedImage = IMG_Load(file.c_str());
    if (loadedImage != NULL) {
        // faz a conversao da imagem
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);

        // carregou na memoria
        data_ = optimizedImage;
        set_frame_size(Vector2D(width(), height()));
        return true;
    }
    else
        return false;
}

// Limpa o surface da imagem usando determinada cor
// Retorna true em caso de sucesso
bool Image::Clear(Image::Color color) {
    SDL_Rect rect = {0, 0, width(), height()};
    return (SDL_FillRect(data_, &rect, color) == 0);
}

// copia esta imagem numa outra
// Retorna true em caso de sucesso
bool Image::DrawTo(Image* dest, const Vector2D& position, int frame_number,
                   Mirror mirror) {
    // frame_number invalido!
    if (frame_number < 0 || frame_number >= FrameCount())
        return false;

    // destino e' valido?
    if(dest == NULL || dest->data_ == NULL)
        return false;

    // obtendo coordenadas do frame_number
    int frame_width = static_cast<int>(frame_size_.x());
    int frame_height = static_cast<int>(frame_size_.y());
    int nx = width()/frame_width;
    int xpos = frame_width * (frame_number % nx);
    int ypos = frame_height * (frame_number / nx);

    // calculando rects...
    SDL_Rect source_rect = {xpos, ypos, frame_width, frame_height};
    SDL_Rect dest_rect = {static_cast<Sint16>(position.x()),
                          static_cast<Sint16>(position.y()),
                          frame_width, frame_height};

    // desenhando...
    SDL_BlitSurface(data_, &source_rect, dest->data_, &dest_rect);

    // sucesso!
    return true;
}

// devolve o numero de frames que esta imagem armazena
int Image::FrameCount() const {
    return static_cast<int>((width()/frame_size_.x())
                            * (height()/frame_size_.y()));
}

// cria uma superficie de video.
// Voce nao deve libera-la porque a SDL ja faz isso
// Devolve true em caso de sucesso
bool Image::CreateVideoSurface(const Vector2D& size, bool fullscreen) {
    int width = static_cast<int>(size.x());
    int height = static_cast<int>(size.y());
    Uint32 flags = SDL_HWSURFACE;

    if(fullscreen)
        flags |= SDL_FULLSCREEN;

    data_ = SDL_SetVideoMode(width, height, VideoManager::COLOR_DEPTH, flags);

    return (data_ != NULL);
}

}  // namespace framework
