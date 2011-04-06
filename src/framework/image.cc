#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include "frame.h"
#include "videomanager.h"
#include "engine.h"
#include "image.h"

namespace framework {

Image::Image() {
    color_ = CreateColor(1.0f, 1.0f, 1.0f);
    alpha_ = 1.0f;
	texture_width_ = texture_height_ = 10;
	texture_ = 0;
    set_frame_size(Vector2D(width(), height()));
}

// Destroys the image and it's texture if delete_texture is true.
// Returns true on success
bool Image::Destroy() {
	if(texture_ != 0) {
        glDeleteTextures(1, &texture_);
		texture_ = 0;
	}
    return true;
}

// Sets what color the image is tinted with during rendering.
void Image::SetColor(Color color) {
    color_ = color;
}

void Image::SetColor(uint32 val) {
    color_.r = ((val & 0xFF0000) >> 16) / 255.0f;
    color_.g = ((val & 0x00FF00) >>  8) / 255.0f;
    color_.b = ((val & 0x0000FF)      ) / 255.0f;
}

// Sets the whole-image alpha value used during rendering.
void Image::SetAlpha(float alpha) {
	if(alpha > 1.0f) // Retro-compatibility
		alpha /= 255.0f;
    alpha_ = alpha;
}

bool Image::DrawTo(const Vector2D& position, int frame_number, 
				   Mirror mirror, const Color& color, float alpha, const Vector2D& draw_size) {
    Vector2D size = draw_size, target = position;

    //Vector2D screen = VIDEO_MANAGER()->video_size();
    Frame bounds = VIDEO_MANAGER()->virtual_bounds();
    //if(target.x > screen.x || target.y > screen.y || target.x + size.x < 0 || target.y + size.y < 0) {
    if (target.x > bounds.right() || target.y > bounds.bottom() ||
        target.x + size.x < bounds.left() || target.y + size.y < bounds.top() ) {
        return true;
    }

    if(mirror & MIRROR_HFLIP) {
        target.x += frame_size().x;
        size.x *= -1;
    }
    if(mirror & MIRROR_VFLIP) {
        target.y += frame_size().y;
        size.y *= -1;
    }

    int frame_width = static_cast<int>(frame_size().x);
    int nx = std::max(width()/frame_width, 1);
    float xpos = frame_size_.x * (frame_number % nx);
    float ypos = frame_size_.y * (frame_number / nx);

    float xend = xpos + frame_size_.x;
    float yend = ypos + frame_size_.y;

    GLuint texture = static_cast<GLuint>(texture_);
    if(texture != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    } else
        glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);

	glColor4f(color.r * color_.r, color.g * color_.g, color.b * color_.b, alpha_ * alpha);

	glPushMatrix();
	glTranslatef( target.x, target.y, 0 );
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
    glDisable(GL_BLEND);
    glPopMatrix();

    return true;

    //return DrawTo(target, flip, frame_number, color, alpha_);
}

void Image::set_frame_size(const Vector2D& size) {
    render_size_ = size;
    frame_size_.x = size.x / texture_width_;
    frame_size_.y = size.y / texture_height_;
}

// devolve o numero de frames que esta imagem armazena
int Image::FrameCount() const {
    Vector2D size = frame_size();
    return static_cast<int>(std::max(1.0f / (size.x * size.y), 1.0f));
}

// Returns a new SDL_Surface with width size.x and height size.y,
// flag SDL_SRCCOLORKEY and depth VideoManager::COLOR_DEPTH.
SDL_Surface* Image::CreateSurface(const Vector2D& size) {
    SDL_Surface *screen = SDL_GetVideoSurface();
    if(screen == NULL)
        return NULL;

    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    int depth = VideoManager::COLOR_DEPTH;
    Uint32 flags = 0;

    SDL_Surface * data = SDL_CreateRGBSurface(flags, width, height, depth,
                                 screen->format->Rmask, screen->format->Gmask,
                                 screen->format->Bmask, screen->format->Amask);
    return data;
}

// Returns a Color with values red, green and blue.
Color Image::CreateColor(float red, float green, float blue) {
    Color color;
    color.r = red;
    color.g = green;
    color.b = blue;
    return color;
}





// Loads the texture from the given surface. Surface is used only as a
// reference and necessary data is copied.
// Returns true on success, false otherwise.
bool Image::LoadFromSurface(SDL_Surface* data, bool linear) {
    fprintf(stderr, "LoadSurface - ");
    if(data == NULL) {
        fprintf(stderr, "No Data\n");
        return false;
    }
    if(texture_ != 0) {
    	fprintf(stderr, "(Del Tex %d) ", texture_);
        glDeleteTextures(1, &texture_);
    }

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
            	fprintf(stderr, "Image BPP of %d unsupported\n", bpp);
                free(raw);
                return false;
                break;
            }
            // Changing pixel order from RGBA to BGRA.
            SDL_GetRGBA( truePixel, data->format, &(dstPixel[2]), &(dstPixel[1]), &(dstPixel[0]), &(dstPixel[3]));
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
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	linear = true; /*POG do Omar pra graficos ficarem normais no note...*/
    if(linear) {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    } else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    }

	glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

    GLenum errorCode = glGetError();
    if ( errorCode != 0 ) {
        if ( errorCode == GL_OUT_OF_MEMORY )
            fprintf(stderr, "Out of texture memory!\n");
        else
        	fprintf(stderr, "Unknown error!\n");
        free(raw);
        return false;
    }
    // Many VGA works faster if you use BGR instead of RGB
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, texture_width_, texture_height_, 0, GL_BGRA, GL_UNSIGNED_BYTE, raw);
    free(raw);

    errorCode = glGetError();
    if ( errorCode != 0 ) {
        if ( errorCode == GL_OUT_OF_MEMORY )
            fprintf(stderr, "Out of texture memory!\n");
        else
        	fprintf(stderr, "Unknown error!\n");
        return false;
    }

    frame_size_ = Vector2D(1.0f, 1.0f);
	render_size_ = Vector2D(texture_width_, texture_height_);

	fprintf(stderr, "Success (ID %d)\n", texture_);

    return true;
}

bool Image::LoadFromFile(std::string filepath) {
    SDL_Surface* data = IMG_Load(filepath.c_str());
    fprintf(stderr, "New Surface from \"%s\", ", filepath.c_str());
    bool result;
    if(data == NULL) {
        fprintf(stderr, "File not found\n");
        result = false;
    } else {
        result = LoadFromSurface(data);
        SDL_FreeSurface(data);
    }
    return result;
}

bool Image::CreateFogTransparency(const Vector2D& size, const Vector2D& ellipse_coef) {
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
            Uint8 alpha = 0;

            // Formulae to detect if the point is inside the ellipse.
            Vector2D dist = Vector2D(j + 0.0f, i + 0.0f) - origin;
            dist.x /= ellipse_coef.x;
            dist.y /= ellipse_coef.y;
            float distance = Vector2D::InnerProduct(dist, dist);
            if(distance <= 1)
                alpha = static_cast<Uint8>(SDL_ALPHA_OPAQUE * exp(-distance * 5.5412635451584261462455391880218));
            pixels[i * width + j] = SDL_MapRGBA(data->format, 0, 0, 0, alpha);
        }
    }
    SDL_UnlockSurface(data);
    bool ret = LoadFromSurface(data, true);
    SDL_FreeSurface(data);
    return ret;
}



Vector2D Image::frame_size() const {
    Vector2D size;
    size.x = frame_size_.x * texture_width_;
    size.y = frame_size_.y * texture_height_;
    return size;
}

}  // namespace framework
