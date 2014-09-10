#include "hudimagefactory.h"

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/ui/drawable/texturedrectangle.h>

using namespace ugdk;

namespace utils {

HudImageFactory::HudImageFactory() {}

TexturedRectangle* HudImageFactory::LifeBarImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_life_bar.png"));
}
TexturedRectangle* HudImageFactory::ManaBarImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_mana_bar.png"));
}
TexturedRectangle* HudImageFactory::TotemImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_totem.png"));
}
TexturedRectangle* HudImageFactory::TotemBottomImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_totem_bottom.png"));
}
TexturedRectangle* HudImageFactory::EyeImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_eye.png"));
}
TexturedRectangle* HudImageFactory::BackImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_horizontal_barx10.png"));
}
TexturedRectangle* HudImageFactory::MummyCounterImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_mummy_counter.png"));
}
TexturedRectangle* HudImageFactory::FireballIconImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_magic_fire.png"));
}
TexturedRectangle* HudImageFactory::EarthquakeIconImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_magic_rock.png"));
}
TexturedRectangle* HudImageFactory::LightningIconImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_magic_lightning.png"));
}
TexturedRectangle* HudImageFactory::LightIconImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_magic_light.png"));
}
TexturedRectangle* HudImageFactory::MeteorIconImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_magic_meteor.png"));
}
TexturedRectangle* HudImageFactory::SandstormIconImage() {
    return new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/hud_magic_sandstorm.png"));
}
}

