#include "sprite.h"
#include "engine.h"
#include "videomanager.h"
#include "animation.h"

namespace framework {

Sprite::~Sprite() {
    if (animation_) delete animation_;
    if (delete_image_ && image_) delete image_;
}

void Sprite::Initialize(Image *image, bool delete_image)
{
    image_ = image;
  	set_zindex(0.0f);
    visible_ = true;
    animation_ = new Animation(50, 0, -1);
    hotspot_ = position_ = Vector2D(0,0);
    mirror_ = Image::MIRROR_NONE;
    delete_image_ = delete_image;
}

void Sprite::SelectAnimation(Animation *animation) {
    animation_ = animation;
}

void Sprite::Render(Image *back_buffer, Vector2D &offset, float color_coef) {
    if (visible_) {
        int frame_number = animation_->get_current_frame();
        image_->DrawTo(position_ - hotspot_ - offset, frame_number,
                       mirror_, color_coef);
    }
}

void Sprite::Update(float delta_t) {
    animation_->Update(delta_t);
}

bool Sprite::CompareByZIndex(Sprite *a, Sprite *b) {
    return a->zindex() < b->zindex();
}

}
