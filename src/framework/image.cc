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
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
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
        optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
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
    if (frame_number < 0 || frame_number >= FrameCount()){
        printf("width %d height %d x %f y %f",width(),height(),frame_size_.x,frame_size_.y);
        printf("%d frame image %d count\n",frame_number,FrameCount());exit(-1);

        return false;
    }

    // destino e' valido?
    if(dest == NULL || dest->data_ == NULL){
        printf("%d FERROU\n",frame_number);exit(-1);
        return false;
    }

    // obtendo coordenadas do frame_number
    int frame_width = static_cast<int>(frame_size_.x);
    int frame_height = static_cast<int>(frame_size_.y);
    int nx = width()/frame_width;
    int xpos = frame_width * (frame_number % nx);
    int ypos = frame_height * (frame_number / nx);

    // calculando rects...
    SDL_Rect source_rect = {xpos, ypos, frame_width, frame_height};
    SDL_Rect dest_rect = {static_cast<Sint16>(position.x + 0.5),
                          static_cast<Sint16>(position.y + 0.5),
                          frame_width, frame_height};

    // desenhando...
    SDL_BlitSurface(data_, &source_rect, dest->data_, &dest_rect);

    // sucesso!
    return true;
}

// devolve o numero de frames que esta imagem armazena
int Image::FrameCount() const {
    int size_x = static_cast<int> (frame_size_.x);
    int size_y = static_cast<int> (frame_size_.y);
    return (width()/size_x) * (height()/size_y);
}

// cria uma superficie de video.
// Voce nao deve libera-la porque a SDL ja faz isso
// Devolve true em caso de sucesso
bool Image::CreateVideoSurface(const Vector2D& size, bool fullscreen) {
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    Uint32 flags = SDL_HWSURFACE;

    if(fullscreen)
        flags |= SDL_FULLSCREEN;

    data_ = SDL_SetVideoMode(width, height, VideoManager::COLOR_DEPTH, flags);

    return (data_ != NULL);
}

bool Image::CreateFogTransparency(const Vector2D& size, const Vector2D& origin, const Vector2D& ellipse_coef, float radius) {
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    Uint32 flags = SDL_SRCALPHA;

    // Cria uma superficie nova. Como nao tem como criar uma RGBA diretamente, cria primeiro uma RGB...
    SDL_Surface* original_surface = SDL_CreateRGBSurface(flags, width, height, VideoManager::COLOR_DEPTH, 0, 0, 0, 0xff);
    if(original_surface == NULL)
        return false;

    // E depois converte para RGBA
    data_ = SDL_DisplayFormatAlpha(original_surface);
    SDL_FreeSurface(original_surface);
    if(data_ == NULL)
        return false;

    set_frame_size(Vector2D(this->width(), this->height()));

    // Trava para poder manipular pixels
    SDL_LockSurface(data_);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Uint8 alpha = SDL_ALPHA_OPAQUE;

            // Formula para detectar se o ponto ta na elipse e outras coisas. Melhorias por favor!
            Vector2D dist = Vector2D(j, i) - origin;
            float distance = Vector2D(dist.x*ellipse_coef.x, dist.y*ellipse_coef.y).length();
            if(distance <= radius)
                alpha = static_cast<Uint8>(SDL_ALPHA_OPAQUE * distance/radius);
            (static_cast<Uint32*>(data_->pixels))[i * width + j] = SDL_MapRGBA(data_->format, 0, 0, 0, alpha);
        }
    }
    SDL_UnlockSurface(data_);
    return true;
}

}  // namespace framework
