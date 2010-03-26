#include "sprite.h"

namespace framework {

Sprite::Sprite(Image *image) {
    Initialise(image);
}

Sprite::Sprite(Image *image, Vector2D position) {
    Initialise(image);
    position_ = position;
}
void Sprite::Initialise(Image *image)
{
    image_ = image;
    visible_ = true;
    animation_ = new Animation(50, 0, -1);
}



void Sprite::SelectAnimation(Animation *animation) {
    animation_ = animation;
}

void Sprite::Render(Image *back_buffer) {
    if (visible_) {
        int frame_number = animation_->get_current_frame();
        image_->DrawTo(back_buffer, position_, frame_number, mirror_);
    }
}

void Sprite::Update(float delta_t) {
    animation_->Update(delta_t);
}
}

