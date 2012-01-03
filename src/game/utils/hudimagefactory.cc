#include "hudimagefactory.h"

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

using namespace ugdk;

namespace utils {

HudImageFactory::HudImageFactory() {}

TexturedRectangle* HudImageFactory::LifeBarImage() {
    return new TexturedRectangle(VIDEO_MANAGER()->LoadTexture("data/images/hud_life_bar.png"));
}
TexturedRectangle* HudImageFactory::ManaBarImage() {
    return new TexturedRectangle(VIDEO_MANAGER()->LoadTexture("data/images/hud_mana_bar.png"));
}
TexturedRectangle* HudImageFactory::TotemImage() {
    return new TexturedRectangle(VIDEO_MANAGER()->LoadTexture("data/images/hud_totem.png"));
}
TexturedRectangle* HudImageFactory::TotemBottomImage() {
    return new TexturedRectangle(VIDEO_MANAGER()->LoadTexture("data/images/hud_totem_bottom.png"));
}
TexturedRectangle* HudImageFactory::EyeImage() {
    return new TexturedRectangle(VIDEO_MANAGER()->LoadTexture("data/images/hud_eye.png"));
}
TexturedRectangle* HudImageFactory::BackImage() {
    return new TexturedRectangle(VIDEO_MANAGER()->LoadTexture("data/images/hud_horizontal_barx10.png"));
}
TexturedRectangle* HudImageFactory::MummyCounterImage() {
    return new TexturedRectangle(VIDEO_MANAGER()->LoadTexture("data/images/hud_mummy_counter.png"));
}
TexturedRectangle* HudImageFactory::FireballIconImage() {
    return new TexturedRectangle(VIDEO_MANAGER()->LoadTexture("data/images/hud_magic_fire.png"));
}
TexturedRectangle* HudImageFactory::EarthquakeIconImage() {
    return new TexturedRectangle(VIDEO_MANAGER()->LoadTexture("data/images/hud_magic_rock.png"));
}
TexturedRectangle* HudImageFactory::LightningIconImage() {
    return new TexturedRectangle(VIDEO_MANAGER()->LoadTexture("data/images/hud_magic_lightning.png"));
}
}

