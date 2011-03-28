#include "image.h"
#include "light.h"
#include "engine.h"
#include "videomanager.h"

namespace framework {

Light::Light() : hotspot_(Vector2D(0,0)) {
}

void Light::Render(Vector2D &offset) {
	Image* img = VIDEO_MANAGER()->light_image();
	Vector2D size = VIDEO_MANAGER()->light_size() * 4.0f;
    size.x *= dimension_.x / VIDEO_MANAGER()->light_size().x;
    size.y *= dimension_.y / VIDEO_MANAGER()->light_size().y;
	img->DrawTo(offset - hotspot_ - size * 0.5f, 0, 0, Image::CreateColor(1,1,1), 1, size);
}

}  // namespace framework
