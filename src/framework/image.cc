//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/image.cc
// Implementacao da classe Image.
//

#include <cmath>
#include <algorithm>
#include <iostream>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <cstring>
#include "frame.h"
#include "videomanager.h"
#include "engine.h"
#include "image.h"

namespace framework {

Image::Image() : gl_tex_(0) {
    color_red_ = color_green_ = color_blue_ = color_alpha_ = 1.0f;
    texture_width_ = texture_height_ = 100;
    set_frame_size(Vector2D(10.0f, 10.0f));
}

// Destroi a imagem
// Retorna true em caso de sucesso
bool Image::Destroy() {
    if(gl_tex_ != 0)
        glDeleteTextures(1, &gl_tex_);
    return true;
}

//Define a surface da imagem
//Retorna true em caso de sucesso
bool Image::SetSurface(SDL_Surface* surface) {
    if(surface != NULL)
        return CreateTexture(surface);
    else
        return false;
}
        
bool Image::CreateTexture(SDL_Surface* data) {
    if(data == NULL)
        return false;
    if(gl_tex_ != 0)
        glDeleteTextures(1, &gl_tex_);

    Uint8 *raw = static_cast<Uint8*>( malloc( data->w * data->h * 4 ) );
    Uint8 *dstPixel = raw;

    SDL_LockSurface( data );
    int bpp = data->format->BytesPerPixel;
    Uint8 *srcPixel;
    Uint32 truePixel;

    for ( int i = 0; i < data->h ; i++ ) {
        for ( int j = 0 ; j < data->w ; j++ ) {
            srcPixel = (Uint8 *)data->pixels + i * data->pitch + j * bpp;
            switch (bpp) {
            case 1:
                truePixel = *srcPixel;
                break;

            case 2:
                truePixel = *srcPixel;
                break;

            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                    truePixel = srcPixel[0] << 16 | srcPixel[1] << 8 | srcPixel[2];
                } else {
                    truePixel = srcPixel[0] | srcPixel[1] << 8 | srcPixel[2] << 16;
                }
                break;

            case 4:
                truePixel = srcPixel[0] | srcPixel[1] << 8 | srcPixel[2] << 16 | srcPixel[3] << 24;
                break;

            default:
                std::cout << "Image bpp of " << bpp << "%d unusable" << std::endl;
                free(raw);
                return false;
                break;
            }
            SDL_GetRGBA( truePixel, data->format, &(dstPixel[0]), &(dstPixel[1]), &(dstPixel[2]), &(dstPixel[3]));
            dstPixel++;
            dstPixel++;
            dstPixel++;
            dstPixel++;
        }
    }
    SDL_UnlockSurface( data );
    texture_width_ = data->w;
    texture_height_ = data->h;

    while ( glGetError() ) { ; }

    GLuint texture;
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    GLenum errorCode = glGetError();
    if ( errorCode != 0 ) {
        if ( errorCode == GL_OUT_OF_MEMORY )
            std::cout << "Out of texture memory!" << std::endl;
        free(raw);
        return false;
    }
    gluBuild2DMipmaps( GL_TEXTURE_2D, 4, texture_width_, texture_height_, GL_RGBA, GL_UNSIGNED_BYTE, raw);
    free(raw);

    gl_tex_ = texture;

    errorCode = glGetError();
    if ( errorCode != 0 ) {
        if ( errorCode == GL_OUT_OF_MEMORY )
            std::cout << "Out of texture memory!" << std::endl;
        return false;
    }
    set_frame_size(Vector2D(static_cast<float>(width()),
                            static_cast<float>(height())));

    return true;
}

// Carrega imagem de um arquivo
// Retorna true em caso de sucesso
bool Image::LoadFromFile(const string& file) {
    SDL_Surface *data = IMG_Load(file.c_str());
    bool ret = CreateTexture(data);
    SDL_FreeSurface(data);
    return ret;
}

// Sets what color the image is tinted with during rendering.
void Image::SetColor(Color color) {
    color_red_ = color.r;
    color_green_ = color.g;
    color_blue_ = color.b;
}

// Sets the whole-image alpha value used during rendering.
void Image::SetAlpha(float alpha) {
    if(alpha > 1.0f) // Assuming it's passing a 0-255 value
        alpha /= 255.0f;
    color_alpha_ = alpha;
}
    
// copia esta imagem numa outra
// Retorna true em caso de sucesso
bool Image::DrawTo(Image* dest, const Vector2D& position, int frame_number,
                   Mirror mirror) {

    // frame_number invalido!
    if (frame_number < 0 || frame_number >= FrameCount()){
        std::cout << "width " << width() << " height " << height();
        std::cout << " x: " << frame_size_.x << " | y: " << frame_size_.y << std::endl;
        std::cout << frame_number << " frame, image count: " << FrameCount() << std::endl;
        return false;
    }

    // obtendo coordenadas do frame_number
    int frame_width = static_cast<int>(frame_size_.x);
   //int frame_height = static_cast<int>(frame_size_.y);
    int nx = std::max(width()/frame_width, 1);
    float xpos = internal_frame_size_.x * (frame_number % nx);
    float ypos = internal_frame_size_.y * (frame_number / nx);

    float xend = xpos + internal_frame_size_.x;
    float yend = ypos + internal_frame_size_.y;

    glTranslatef( position.x, position.y, 0 );
    if(gl_tex_ != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, gl_tex_);
    } else
        glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(color_red_, color_green_, color_blue_, color_alpha_);
    glBegin( GL_QUADS ); //Start quad
        //Draw square
        glTexCoord2f(xpos,        ypos);
        glVertex3f(  0,           0,             0 );

        glTexCoord2f(xend,          ypos);
        glVertex3f(  frame_size_.x, 0,             0 );

        glTexCoord2f(xend,          yend);
        glVertex3f(  frame_size_.x, frame_size_.y, 0 );

        glTexCoord2f(xpos,          yend);
        glVertex3f(  0,             frame_size_.y, 0 );
    glEnd(); //End quad
    //Reset
    glLoadIdentity();

    // sucesso!
    return true;
}

void Image::set_frame_size(const Vector2D& size) {
    frame_size_ = size;
    internal_frame_size_.x = size.x / texture_width_;
    internal_frame_size_.y = size.y / texture_height_;
}

// devolve o numero de frames que esta imagem armazena
int Image::FrameCount() const {
    int size_x = static_cast<int> (frame_size_.x);
    int size_y = static_cast<int> (frame_size_.y);
    return std::max((width()/size_x) * (height()/size_y), 1);
}

bool Image::CreateFogTransparency(const Vector2D& ellipse_coef) {
    //int width = static_cast<int>(2.0f * ellipse_coef.x);
    //int height = static_cast<int>(2.0f * ellipse_coef.y);
    int width = static_cast<int>(VIDEO_MANAGER()->video_size().x);
    int height = static_cast<int>(VIDEO_MANAGER()->video_size().y);
    SDL_Surface *screen = SDL_GetVideoSurface();

    SDL_Surface *temp = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, VideoManager::COLOR_DEPTH,
                                 screen->format->Rmask, screen->format->Gmask,
                                 screen->format->Bmask, screen->format->Amask);
    if(temp == NULL)
        return false;
    SDL_Surface *data = SDL_DisplayFormatAlpha(temp);
    SDL_FreeSurface(temp);
    if(data == NULL)
        return false;

    Vector2D origin = VIDEO_MANAGER()->video_size() * 0.5f;

    // Trava para poder manipular pixels
    SDL_LockSurface(data);
    Uint32 *pixels = static_cast<Uint32*>(data->pixels);
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
            pixels[i * width + j] = alpha << data->format->Ashift;
        }
    }
    SDL_UnlockSurface(data);
    bool ret = CreateTexture(data);
    SDL_FreeSurface(data);
    return ret;
}

SDL_Surface* Image::CreateSurface(const Vector2D& size) {
    SDL_Surface *screen = SDL_GetVideoSurface();
    if(screen == NULL)
        return NULL;

    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    int depth = VideoManager::COLOR_DEPTH;
    Uint32 flags = SDL_SRCCOLORKEY;


    SDL_Surface * data = SDL_CreateRGBSurface(flags, width, height, depth,
                                 screen->format->Rmask, screen->format->Gmask,
                                 screen->format->Bmask, screen->format->Amask);

    return data;
}

Color Image::CreateColor(float red, float green, float blue) {
    Color color;
    color.r = red;
    color.g = green;
    color.b = blue;
    return color;
}
Color Image::CreateColor(Uint32 arg) {
    Color color;
    color.r = ((arg & 0xFF0000) >> 4) / 255.0f;
    color.g = ((arg & 0x00FF00) >> 2) / 255.0f;
    color.b = ((arg & 0x0000FF)     ) / 255.0f;
    return color;
}
}  // namespace framework
