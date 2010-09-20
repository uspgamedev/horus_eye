#include <cmath>
#include <cstdio>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include "texture.h"

namespace framework {

Texture::~Texture() {
    if(texture_ != 0)
        glDeleteTextures(1, &texture_);
}

bool Texture::DrawTo(const Vector2D& position,  const Vector2D& size,
        int frame_number, Color color, float alpha) {

    int frame_width = static_cast<int>(frame_size().x);
    int nx = std::max(width()/frame_width, 1);
    float xpos = frame_size_.x * (frame_number % nx);
    float ypos = frame_size_.y * (frame_number / nx);

    float xend = xpos + frame_size_.x;
    float yend = ypos + frame_size_.y;

    glTranslatef( position.x, position.y, 0 );
    GLuint texture = static_cast<GLuint>(texture_);
    if(texture != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    } else
        glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(color.r, color.g, color.b, alpha);
    glBegin( GL_QUADS ); //Start quad
        //Draw square
        glTexCoord2f(xpos,   ypos);
        glVertex2f(  0,      0 );

        glTexCoord2f(xend,   ypos);
        glVertex2f(  size.x, 0 );

        glTexCoord2f(xend,   yend);
        glVertex2f(  size.x, size.y );

        glTexCoord2f(xpos,   yend);
        glVertex2f(  0,      size.y );
    glEnd(); //End quad
    //Reset
    glLoadIdentity();

    return true;
}

// Loads the texture from the given surface. Surface is used only as a
// reference and necessary data is copied.
// Returns true on success, false otherwise.
bool Texture::LoadFromSurface(SDL_Surface* data) {
    if(data == NULL)
        return false;
    if(texture_ != 0)
        glDeleteTextures(1, &texture_);

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
                printf("Image bpp of %d unusable\n", bpp);
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
    texture_ = static_cast<uint32>(texture);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    GLenum errorCode = glGetError();
    if ( errorCode != 0 ) {
        if ( errorCode == GL_OUT_OF_MEMORY )
            printf("Out of texture memory!\n");
        free(raw);
        return false;
    }
    gluBuild2DMipmaps( GL_TEXTURE_2D, 4, texture_width_, texture_height_, GL_RGBA, GL_UNSIGNED_BYTE, raw);
    free(raw);

    errorCode = glGetError();
    if ( errorCode != 0 ) {
        if ( errorCode == GL_OUT_OF_MEMORY )
            printf("Out of texture memory!\n");
        return false;
    }

    frame_size_ = Vector2D(1.0f, 1.0f);

    return true;
}

bool Texture::LoadFromFile(std::string filepath) {
    SDL_Surface* data = IMG_Load(filepath.c_str());
    bool result = LoadFromSurface(data);
    SDL_FreeSurface(data);
    return result;
}

bool Texture::CreateFogTransparency(const Vector2D& size, const Vector2D& ellipse_coef) {
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    SDL_Surface *screen = SDL_GetVideoSurface();

    SDL_Surface *temp = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, screen->format->BitsPerPixel,
                                 screen->format->Rmask, screen->format->Gmask,
                                 screen->format->Bmask, screen->format->Amask);
    if(temp == NULL)
        return false;
    SDL_Surface *data = SDL_DisplayFormatAlpha(temp);
    SDL_FreeSurface(temp);
    if(data == NULL)
        return false;

    Vector2D origin = size * 0.5f;

    // Locks the surface so we can manage the pixel data.
    SDL_LockSurface(data);
    Uint32 *pixels = static_cast<Uint32*>(data->pixels);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Uint8 alpha = SDL_ALPHA_OPAQUE;

            // Formulae to detect if the point is inside the ellipse.
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
    bool ret = LoadFromSurface(data);
    SDL_FreeSurface(data);
    return ret;
}

void Texture::set_frame_size(const Vector2D& size) {
    frame_size_.x = size.x / texture_width_;
    frame_size_.y = size.y / texture_height_;
}

Vector2D Texture::frame_size() const {
    Vector2D size;
    size.x = frame_size_.x * texture_width_;
    size.y = frame_size_.y * texture_height_;
    return size;
}

}
