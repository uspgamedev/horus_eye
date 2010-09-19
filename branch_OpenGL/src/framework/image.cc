#include <cmath>
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

// Destroi a imagem
// Retorna true em caso de sucesso
bool Image::Destroy() {
    if(delete_texture_)
        delete texture_;
    return true;
}

// Sets what color the image is tinted with during rendering.
void Image::SetColor(Color color) {
    color_ = color;
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

void Image::set_frame_size(const Vector2D& size) {
    render_size_ = size;
    texture_->set_frame_size(size);
}

// devolve o numero de frames que esta imagem armazena
int Image::FrameCount() const {
    Vector2D size = texture_->frame_size();
    return static_cast<int>(std::max(1.0f / (size.x * size.y), 1.0f));
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
    bool ret = texture_->LoadFromSurface(data);
    set_frame_size(Vector2D(texture_->width(), texture_->height()));
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

bool Image::Clear(Uint32 val) {
    Color color;
	color.r = ((val & 0xFF0000) >> 16) / 255.0f;
	color.g = ((val & 0x00FF00) >>  8) / 255.0f;
	color.b = ((val & 0x0000FF)      ) / 255.0f;
    SetColor(color);
	return true;
}

}  // namespace framework
