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

class Texture;

// Representa uma imagem na memoria
class Image {
  friend class VideoManager;
  public:
    typedef Uint8 Mirror;
    static const Mirror MIRROR_NONE = 0;
    static const Mirror MIRROR_HFLIP = 1;
    static const Mirror MIRROR_VFLIP = 2;

    Image(Texture* texture = NULL, bool delete_texture = true);
    ~Image() {}

    bool Destroy();

    void SetColor(Color color);
    bool SetColor(uint32 color);
    void SetAlpha(float alpha);

    int width() const;
    int height() const;

    void set_render_size(const Vector2D& size) { render_size_ = size; }
    Vector2D render_size() const { return render_size_; }
    int FrameCount() const;

    bool DrawTo(const Vector2D& position, int frame_number, Mirror mirror);

    static SDL_Surface* CreateSurface(const Vector2D& size);
    static Color CreateColor(float red, float green, float blue);
	
    void set_frame_size(const Vector2D& size);

	// Deprecated. Calls set_frame_size and always return true.
    bool Create(const Vector2D& size, uint32 flags = 0) {
        set_frame_size(size); return true;
    }

    // Deprecated. Calls SetColor and always return true.
    bool Clear(Uint32 color) { SetColor(color); return true; }

    // Deprecated. Calls DrawTo with the dest argument.
    bool DrawTo(Image* dest, const Vector2D& position, int frame_number,
                Mirror mirror) {
        return DrawTo(position, frame_number, mirror);
    }

    // Deprecated. Wrapper for render_size()
    Vector2D frame_size() const { return render_size_; }

  private:
    Texture* texture_;
    Vector2D render_size_;
    Color color_;
    float alpha_;
    bool delete_texture_;
};

}  // namespace framework

#endif
