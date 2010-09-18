#ifndef HORUSEYE_FRAMEWORK_IMAGE_H_
#define HORUSEYE_FRAMEWORK_IMAGE_H_

#include <string>
#include <map>
#include <SDL/SDL.h>
#include "vector2D.h"
#include "types.h"
#include "videomanager.h"
using std::string;

namespace framework {

// Representa uma imagem na memoria
class Image {
  friend class VideoManager;
  public:
    typedef Uint8 Mirror;
    static const Mirror MIRROR_NONE = 0;
    static const Mirror MIRROR_HFLIP = 1;
    static const Mirror MIRROR_VFLIP = 2;

    Image();
    ~Image() {}

    bool CreateFogTransparency(const Vector2D& ellipse_coef);
    bool Destroy();

    void SetColor(Color color);
    void SetAlpha(float alpha);

    int width() const { return texture_width_; }
    int height() const { return texture_height_; }

    void set_frame_size(const Vector2D& size);
    Vector2D frame_size() const { return frame_size_; }
    int FrameCount() const;

    bool DrawTo(Image* dest, const Vector2D& position, int frame_number,
                Mirror mirror);

    bool SetSurface(SDL_Surface *surface);

    static SDL_Surface* CreateSurface(const Vector2D& size);
    static Color CreateColor(float red, float green, float blue);
	
	// Backwards compatibility, do not use
    bool Create(const Vector2D& size, uint32 flags = 0) {
        set_frame_size(size); return true;
    }
    bool Clear(Uint32 color);

  private:
    Vector2D frame_size_, internal_frame_size_;
    int texture_width_, texture_height_;
    uint32 gl_tex_;
    float color_red_, color_green_, color_blue_, color_alpha_;

    bool CreateTexture(SDL_Surface* data);

    // apenas o VideoManager acessa
    bool LoadFromFile(const string& file);
};

}  // namespace framework

#endif
