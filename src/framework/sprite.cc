#include <SDL/SDL_opengl.h>
#include "sprite.h"
#include "drawable.h"
#include "light.h"
#include "engine.h"
#include "videomanager.h"
#include "animation.h"

namespace framework {

Sprite::Sprite() : light_(NULL), modifier_(), delete_image_(false) {}

Sprite::~Sprite() {
    if (animation_) delete animation_;
    if (delete_image_ && image_) delete image_;
}

void Sprite::Initialize(Drawable *image, AnimationSet *set, bool delete_image)
{
    image_ = image;
  	set_zindex(0.0f);
    visible_ = true;
    animation_ = new Animation(10, set);
    hotspot_ = position_ = Vector2D(0,0);
    //mirror_ = Image::MIRROR_NONE;
    delete_image_ = delete_image;
	size_ = image->render_size();
}

void Sprite::Render() {
    if (visible_) {
        int frame_number = animation_->get_current_frame();
        Modifier *modifier = animation_->get_current_modifier();
        image_->DrawTo(position_ - hotspot_, frame_number, modifier, size_);
    }
}

void Sprite::RenderLight(Vector2D &offset) {
    if (visible_ && light_) {
		Vector2D pos = position_ - offset;
        light_->Render(pos);
    }
}

void Sprite::Update(float delta_t) {
    animation_->Update(delta_t);
}

bool Sprite::CompareByZIndex(Sprite *a, Sprite *b) {
    return a->zindex() < b->zindex();
}

}
