#include "hudimagefactory.h"
#include "../config.h"
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/image.h"
#include "constants.h"

using namespace ugdk;
namespace utils {
HudImageFactory::HudImageFactory(){
    life_bar_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_life_bar.png");
    if(life_bar_image_) life_bar_image_->set_frame_size(Vector2D(Constants::LIFE_BAR_WIDTH, Constants::LIFE_BAR_HEIGHT));

    mana_bar_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_mana_bar.png");
    if(mana_bar_image_) mana_bar_image_->set_frame_size(Vector2D(Constants::MANA_BAR_WIDTH, Constants::MANA_BAR_HEIGHT));

    totem_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_totem.png");

    totem_bottom_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_totem_bottom.png");

    eye_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_eye.png");
    if(eye_image_) eye_image_->set_frame_size(Vector2D(200, 80));

    back_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_horizontal_barx10.png");
    if(back_image_) back_image_->set_frame_size(Vector2D(1150, 80));

    mummy_counter_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_mummy_counter.png");
    if(mummy_counter_image_) mummy_counter_image_->set_frame_size(Vector2D(190, 138));

    earthquake_icon_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_magic_rock.png");
    if(earthquake_icon_image_) earthquake_icon_image_->set_frame_size(Vector2D(25, 35));
    
    fireball_icon_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_magic_fire.png");
    if(fireball_icon_image_) fireball_icon_image_->set_frame_size(Vector2D(25, 35));

    lightning_icon_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_magic_lightning.png");
    if(lightning_icon_image_) fireball_icon_image_->set_frame_size(Vector2D(25, 32));
}

Image* HudImageFactory::LifeBarImage() {
    return life_bar_image_;
}
Image* HudImageFactory::ManaBarImage() {
    return mana_bar_image_;
}
Image* HudImageFactory::TotemImage() {
    return totem_image_;
}
Image* HudImageFactory::TotemBottomImage() {
    return totem_bottom_image_;
}
Image* HudImageFactory::EyeImage() {
    return eye_image_;
}
Image* HudImageFactory::BackImage() {
    return back_image_;
}
Image* HudImageFactory::MummyCounterImage() {
    return mummy_counter_image_;
}
Image* HudImageFactory::FireballIconImage() {
    return fireball_icon_image_;
}
Image* HudImageFactory::EarthquakeIconImage() {
    return earthquake_icon_image_;
}
Image* HudImageFactory::LightningIconImage() {
    return lightning_icon_image_;
}
}

