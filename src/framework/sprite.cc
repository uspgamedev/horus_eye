#include "sprite.h"

namespace framework {

/*
Sprite::Sprite(Image *image) : zindex_(0.0f) {
    Initialise(image);
}

Sprite::Sprite(Image *image, Vector2D position) : zindex_(0.0f) {
    Initialise(image);
    position_ = position;
}
*/

void Sprite::Initialise(Image *image)
{
    image_ = image;
  	set_zindex(0.0f);
    visible_ = true;
    animation_ = new Animation(50, 0, -1);
    hotspot_ = position_ = Vector2D(0,0);
}

void Sprite::SelectAnimation(Animation *animation) {
    animation_ = animation;
}

void Sprite::Render(Image *back_buffer) {
    if (visible_) {
        int frame_number = animation_->get_current_frame();
        image_->DrawTo(back_buffer, position_ - hotspot_, frame_number, mirror_);
    }
}

void Sprite::Update(float delta_t) {
    animation_->Update(delta_t);
}

bool Sprite::CompareByZIndex(Sprite *a, Sprite *b) {
    return a->zindex() < b->zindex();
}

}
