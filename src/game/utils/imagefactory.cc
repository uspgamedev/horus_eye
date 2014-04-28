#include "imagefactory.h"
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/spritesheet.h>
#include "game/constants.h"

using ugdk::math::Vector2D;
using ugdk::graphic::Drawable;
using ugdk::graphic::Spritesheet;

namespace utils {

Spritesheet* ImageFactory::HeroImage() {
    return ugdk::resource::GetSpritesheetFromTag("hero");
}

Spritesheet* ImageFactory::MummyImage(){
    return ugdk::resource::GetSpritesheetFromTag("mummy_basic");
}

Spritesheet* ImageFactory::BigMummyImage() {
    return ugdk::resource::GetSpritesheetFromTag("mummy_big");
}

Spritesheet* ImageFactory::PharaohImage(){
    return ugdk::resource::GetSpritesheetFromTag("pharaoh");
}

Spritesheet* ImageFactory::RangedMummyImage(){
    return ugdk::resource::GetSpritesheetFromTag("mummy_ranged");
}

Spritesheet* ImageFactory::MagicMissileImage() {
    return ugdk::resource::GetSpritesheetFromTag("magic_missile");
}

Spritesheet* ImageFactory::FireballImage() {
    return ugdk::resource::GetSpritesheetFromTag("fireball");
}

Spritesheet* ImageFactory::MummyProjectileImage() {
    return ugdk::resource::GetSpritesheetFromTag("mummy_projectile");
}

Spritesheet* ImageFactory::DoorImage() {
    return ugdk::resource::GetSpritesheetFromTag("stairs");
}

Spritesheet* ImageFactory::WallImage() {
    return ugdk::resource::GetSpritesheetFromTag("wall");
}

Spritesheet* ImageFactory::EntryImage() {
    return ugdk::resource::GetSpritesheetFromTag("door");
}

Spritesheet* ImageFactory::LightningImage() {
    return ugdk::resource::GetSpritesheetFromTag("lightning_bolt");
}

Spritesheet* ImageFactory::LightImage() {
    return ugdk::resource::GetSpritesheetFromTag("light");
}

Spritesheet* ImageFactory::TileSwitchImage() {
    return ugdk::resource::GetSpritesheetFromTag("tile_switch");
}

ugdk::internal::GLTexture* ImageFactory::LifePotionImage() {
    return ugdk::resource::GetTextureFromFile("images/life_potion2.png");
}

ugdk::internal::GLTexture* ImageFactory::ManaPotionImage() {
    return ugdk::resource::GetTextureFromFile("images/mana_potion.png");
}

}

