//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/image.cc
// Implementacao da classe Image.
//

#include <cmath>
#include <algorithm>
#include <SDL/SDL_image.h>
#include "frame.h"
#include "videomanager.h"
#include "image.h"

namespace framework {

// Cria uma imagem com tamanho size.
// Retorna true em caso de sucesso
bool Image::Create(const Vector2D& size, uint32 flags) {
    SDL_Surface *screen = SDL_GetVideoSurface();
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    int depth = VideoManager::COLOR_DEPTH;

    if(screen == NULL)
        return false;

    data_ = SDL_CreateRGBSurface(flags, width, height, depth,
                                 screen->format->Rmask, screen->format->Gmask,
                                 screen->format->Bmask, screen->format->Amask);

    set_frame_size(Vector2D(static_cast<float>(this->width()), 
							static_cast<float>(this->height())));
    return (data_ != NULL);
}
bool Image::Create(const Vector2D& size) {
    return Create(size, SDL_HWSURFACE | SDL_SRCCOLORKEY);
}

// Destroi a imagem
// Retorna true em caso de sucesso
bool Image::Destroy() {
    if (data_ != NULL)
        SDL_FreeSurface(data_);

    return true;
}

//Define a surface da imagem
//Retorna true em caso de sucesso
bool Image::setSurface(SDL_Surface* surface) {
    if(surface != NULL){
        data_ = surface;
        set_frame_size(Vector2D(width(), height()));
        return true;
    }
    else
        return false;
}
        
//Faz um blit de uma surface na surface da imagem
//Retorna true em caso de sucesso
bool Image::blitSurface(SDL_Surface* surface, SDL_Rect* srcrect, SDL_Rect* dstrect) {
    if(surface != NULL && dstrect != NULL){
        if(SDL_BlitSurface(surface, srcrect, data_, dstrect)==0){
            return true;
        }
        else
            return false;
    }
    else
        return false;
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
        set_frame_size(Vector2D(static_cast<float>(width()), 
								static_cast<float>(height())));
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

//Define a cor de transparencia
//Retorna true em caso de sucesso
bool Image::setColorKey(SDL_Color color) { 
    return SDL_SetColorKey(data_, SDL_SRCCOLORKEY, SDL_MapRGB(data_->format, color.r, color.g, color.b));
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
    return std::max((width()/size_x) * (height()/size_y), 1);
}

void Image::Optimize() {
    SDL_Surface *optimizedImage = SDL_DisplayFormatAlpha(data_);
    SDL_FreeSurface(data_);
    data_ = optimizedImage;
}

void Image::MergeTransparency(Image* target, Vector2D& offset) {
    int offset_x = static_cast<int>(offset.x),
        offset_y = static_cast<int>(offset.y);

    int start_x = std::max(0, offset_x);
    int max_x = std::min(offset_x + target->width(), width());
    int max_y = std::min(offset_y + target->height(), height());

    SDL_LockSurface(data_);
    SDL_LockSurface(target->data_);

    // Avoids converting the arrays every iteration of the nested for-loop,
    // increasing the speed a bit.
    Uint32 *source_pixels = static_cast<Uint32*>(data_->pixels),
           *target_pixels = static_cast<Uint32*>(target->data_->pixels);

    for (int source_y = std::max(0, offset_y); source_y < max_y; ++source_y) {

        // Avoids recalculating these indexes during the second for-loop, speeding
        // the process a bit more.
        int source_index = source_y * data_->w + start_x;
        int target_index = (source_y - offset_y) * target->data_->w + (start_x - offset_x);

        for (int source_x = start_x; source_x < max_x; ++source_x, ++source_index, ++target_index) {
			Uint32 this_pixel = source_pixels[source_index];
            this_pixel = this_pixel & (data_->format->Amask);
            this_pixel = this_pixel >> data_->format->Ashift;
            this_pixel = this_pixel << data_->format->Aloss;
            Uint8 alpha = static_cast<Uint8>(this_pixel);

            Uint32 that_pixel = target_pixels[target_index];
            that_pixel = that_pixel & (target->data_->format->Amask);
            that_pixel = that_pixel >> target->data_->format->Ashift;
            that_pixel = that_pixel << target->data_->format->Aloss;
            Uint8 thatAlpha = static_cast<Uint8>(that_pixel);

            // On SDL, alpha is stored such as that SDL_ALPHA_OPAQUE is the max value (255).
            // In order to create the effect of multiple lights, we must first invert the values
            // (so 255 means fully tranparent), add then and then convert it back to what SDL uses.
            Uint16 invert = SDL_ALPHA_OPAQUE + SDL_ALPHA_OPAQUE;
            invert -= (alpha + thatAlpha);
			if(invert > SDL_ALPHA_OPAQUE)
				invert = 0;
			else
				invert = SDL_ALPHA_OPAQUE - invert;

			source_pixels[source_index] = static_cast<Uint8>(invert) << data_->format->Ashift;
        }
    }

    SDL_UnlockSurface(target->data_);
    SDL_UnlockSurface(data_);
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

bool Image::CreateFogTransparency(const Vector2D& ellipse_coef) {
    int width = static_cast<int>(2.0f * ellipse_coef.x);
    int height = static_cast<int>(2.0f * ellipse_coef.y);
    if(!Create(ellipse_coef * 2.0f))
        return false;

    Optimize();

    Vector2D origin = ellipse_coef;

    // Trava para poder manipular pixels
    SDL_LockSurface(data_);
    Uint32 *pixels = static_cast<Uint32*>(data_->pixels);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Uint8 alpha = SDL_ALPHA_OPAQUE;

            // Formula para detectar se o ponto ta na elipse e outras coisas. Melhorias por favor!
            Vector2D dist = Vector2D(j + 0.0f, i + 0.0f) - origin;
            dist.x /= ellipse_coef.x;
            dist.y /= ellipse_coef.y;
            float distance = Vector2D::InnerProduct(dist, dist);
            if(distance <= 1)
                alpha -= static_cast<Uint8>(SDL_ALPHA_OPAQUE * exp(-distance * 5.5412635451584261462455391880218));
            pixels[i * width + j] = alpha << data_->format->Ashift;
        }
    }
    SDL_UnlockSurface(data_);
    return true;
}

}  // namespace framework
