#include "sprite.h";

namespace framework {

Sprite::Sprite(Image *image) {
    image_ = image;
    visible_ = true;
    animation_ = NULL;
}

Sprite::Sprite(Image *image, Vector2D position) {
    image_ = image;
    visible_ = true;
    animation_ = NULL;
    position_ = position; 
}

void Sprite::SelectAnimation(Animation *animation) {
    animation_ = animation;
}

void Sprite::Render(Image *back_buffer) {
    if (visible_) {
        int frame_number = animation_.get_current_frame();
        //FIXME
        Image::Mirror mirror = NULL; 
        image_.DrawTo(back_buffer, position, frame_number, mirror);
    }
}

virtual void Sprite::Update(float delta_t) {
    animation_.Update(delta_t);
}
}

