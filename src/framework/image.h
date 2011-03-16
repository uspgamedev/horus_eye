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

//class Texture;

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
    bool Destroy();

    void SetColor(Color color);
    void SetColor(uint32 color);
    void SetAlpha(float alpha);

	uint32 texture() { return texture_; }
    int width() const { return texture_width_; }
    int height() const { return texture_height_; }

	void set_frame_size(const Vector2D& size);
	Vector2D frame_size() const;

    void set_render_size(const Vector2D& size) { render_size_ = size; }
    Vector2D render_size() const { return render_size_; }
    
	int FrameCount() const;

    bool DrawTo(const Vector2D& position, int frame_number, Mirror mirror, 
		const Color& color, float alpha, const Vector2D& draw_size);

    bool LoadFromSurface(SDL_Surface* data, bool linear = false);
    bool LoadFromFile(std::string filepath);
    bool CreateFogTransparency(const Vector2D& size, const Vector2D& ellipse_coef);

    static SDL_Surface* CreateSurface(const Vector2D& size);
    static Color CreateColor(float red, float green, float blue);

	// Deprecated. Calls set_frame_size and always return true.
    bool Create(const Vector2D& size, uint32 flags = 0) {
        set_frame_size(size); return true;
    }

    // Deprecated. Calls SetColor and always return true.
    bool Clear(Uint32 color) { SetColor(color); return true; }

    // Deprecated. Calls DrawTo with the dest argument.
    bool DrawTo(Image* dest, const Vector2D& position, int frame_number,
                Mirror mirror) {
        return DrawTo(position, frame_number, mirror, CreateColor(1, 1, 1), 1.0f, frame_size());
    }

  private:
    Vector2D render_size_;
    Color color_;
    float alpha_;
    bool delete_texture_;

// TEXTURE PART
    uint32 texture_;
    int texture_width_, texture_height_;
    Vector2D frame_size_;
};

}  // namespace framework

#endif
