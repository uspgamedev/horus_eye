#include "sprite.h"
#include "engine.h"
#include "videomanager.h"
#include "animation.h"

namespace framework {

Sprite::~Sprite() {
    if (animation_) delete animation_;
}

void Sprite::Initialize(Image *image)
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

void Sprite::Render(Image *back_buffer, Vector2D &offset) {
    if (visible_) {
        if(image_ != NULL) {
            int frame_number = animation_->get_current_frame();
            image_->DrawTo(back_buffer, position_ - hotspot_ - offset,
                    frame_number, mirror_);
        } else {
            VIDEO_MANAGER()->blank_image()->DrawTo(back_buffer,
                    position_ - hotspot_ - offset, 0, 0);
        }
    }
}

void Sprite::Update(float delta_t) {
    animation_->Update(delta_t);
}

bool Sprite::CompareByZIndex(Sprite *a, Sprite *b) {
    return a->zindex() < b->zindex();
}

}
