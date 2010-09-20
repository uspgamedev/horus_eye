#ifndef HORUSEYE_FRAMEWORK_TEXTURE_H_
#define HORUSEYE_FRAMEWORK_TEXTURE_H_

#include <string>
#include "vector2D.h"
#include "types.h"

namespace framework {

// Represents an OpenGL texture
class Texture {
  public:
    Texture() : texture_(0) {
        texture_width_ = texture_height_ = 10;
        frame_size_ = Vector2D(1.0f, 1.0f);
    }
    ~Texture();

    bool DrawTo(const Vector2D& position, const Vector2D& size,
            int frame_number, Color color, float alpha);

    bool LoadFromSurface(SDL_Surface* data);
    bool LoadFromFile(std::string filepath);
    bool CreateFogTransparency(const Vector2D& size, const Vector2D& ellipse_coef);

    uint32 texture() { return texture_; }
    int width() const { return texture_width_; }
    int height() const { return texture_height_; }

    void set_frame_size(const Vector2D& size);
    Vector2D frame_size() const;

  private:
    uint32 texture_;
    int texture_width_, texture_height_;
    Vector2D frame_size_;
};

}

#endif /* HORUSEYE_FRAMEWORK_TEXTURE_H_ */
