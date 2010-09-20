#include <algorithm>
#include <iostream>
#include <SDL/SDL_image.h>
#include <cstring>
#include "frame.h"
#include "videomanager.h"
#include "engine.h"
#include "image.h"
#include "texture.h"

namespace framework {

Image::Image(Texture* texture, bool delete_texture) {
    color_ = CreateColor(1.0f, 1.0f, 1.0f);
    alpha_ = 1.0f;
    if(texture == NULL)
        texture = new Texture;
    texture_ = texture;
    set_render_size(Vector2D(texture_->width(), texture_->height()));
    delete_texture_ = delete_texture;
}

// Destroys the image and it's texture if delete_texture is true.
// Returns true on success
bool Image::Destroy() {
    if(delete_texture_)
        delete texture_;
    return true;
}

// Sets what color the image is tinted with during rendering.
void Image::SetColor(Color color) {
    color_ = color;
}

void Image::SetColor(Uint32 val) {
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
    
// copia esta imagem numa outra
// Retorna true em caso de sucesso
bool Image::DrawTo(const Vector2D& position, int frame_number,
                   Mirror mirror) {
    Vector2D flip = render_size(), target = position;
    if(mirror & MIRROR_HFLIP) {
        target.x += frame_size().x;
        flip.x *= -1;
    }
    if(mirror & MIRROR_VFLIP) {
        target.y += frame_size().y;
        flip.y *= -1;
    }
    return texture_->DrawTo(target, flip, frame_number, color_, alpha_);
}

int Image::width() const { return texture_->width(); }
int Image::height() const { return texture_->height(); }

// Wrapper for set_render_size and Texture::set_frame_size
void Image::set_frame_size(const Vector2D& size) {
    render_size_ = size;
    texture_->set_frame_size(size);
}

// devolve o numero de frames que esta imagem armazena
int Image::FrameCount() const {
    Vector2D size = texture_->frame_size();
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
    Uint32 flags = SDL_SRCCOLORKEY;


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

}  // namespace framework
