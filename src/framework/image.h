#ifndef HORUSEYE_FRAMEWORK_IMAGE_H_
#define HORUSEYE_FRAMEWORK_IMAGE_H_

#include <string>
#include <map>
#include <SDL/SDL.h>
#include "vector2D.h"
#include "types.h"
#include "drawable.h"
#include "videomanager.h"

namespace framework {

// Square with texture implementation of a Drawable.
class Image : public Drawable {
  friend class VideoManager;
  public:
    Image();
	~Image() {}
    bool Destroy();

	int width() const { return texture_width_; }
    int height() const { return texture_height_; }

	// Sets the frame_size to use with the texture.
	void set_frame_size(const Vector2D& size);
	Vector2D frame_size() const;

	// Sets the render_size. It's not used by the Image.
    void set_render_size(const Vector2D& size) { render_size_ = size; }
    Vector2D render_size() const { return render_size_; }

	// Sets what color the image is tinted with during rendering.
	void set_color(const Color &color) { color_ = color; }
	void set_color(const float r, const float g, const float b) {
		color_ = Image::CreateColor(r, g, b);
	}
	Color color() const { return color_; }

	// Sets the whole-image alpha value used during rendering.
	void set_alpha(const float alpha) { alpha_ = alpha; }
	float alpha() const { return alpha_; }
    
	int FrameCount() const;

	// Draws a 1.0f x 1.0f square at origin with the given frame_number. 
	// Warning: Uses the currently set color.
	void RawDraw(int frame_number);

	// Draws at position, a draw_size square with the given frame_number
	// modified by mirror and both the image and given color and alpha.
    bool DrawTo(const Vector2D& position, int frame_number, Mirror mirror, 
		const Color& color, float alpha, const Vector2D& draw_size);

	// Creates a texture from the given SDL_Surface. Overwrites any current texture data.
    bool LoadFromSurface(SDL_Surface* data, bool linear = false);

	// Loads the image at filepath and creates a texture from the data.
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

	// Deprecated. Calls set_color.
	void SetColor(uint32 color);

  private:
    Color color_;
    float alpha_;
	Vector2D render_size_;

// TEXTURE PART
    uint32 texture_;
    int texture_width_, texture_height_;
    Vector2D frame_size_;
};

}  // namespace framework

#endif
