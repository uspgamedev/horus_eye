#include "imagefactory.h"
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include "game/constants.h"

using ugdk::math::Vector2D;
using ugdk::graphic::Drawable;
using ugdk::graphic::TextureAtlas;

namespace utils {

TextureAtlas* ImageFactory::HeroImage() {
    return ugdk::resource::GetTextureAtlasFromTag("hero");
}

TextureAtlas* ImageFactory::MummyImage(){
    return ugdk::resource::GetTextureAtlasFromTag("mummy_basic");
}

TextureAtlas* ImageFactory::BigMummyImage() {
    return ugdk::resource::GetTextureAtlasFromTag("mummy_big");
}

TextureAtlas* ImageFactory::PharaohImage(){
    return ugdk::resource::GetTextureAtlasFromTag("pharaoh");
}

TextureAtlas* ImageFactory::RangedMummyImage(){
    return ugdk::resource::GetTextureAtlasFromTag("mummy_ranged");
}

TextureAtlas* ImageFactory::MagicMissileImage() {
    return ugdk::resource::GetTextureAtlasFromTag("magic_missile");
}

TextureAtlas* ImageFactory::FireballImage() {
    return ugdk::resource::GetTextureAtlasFromTag("fireball");
}

TextureAtlas* ImageFactory::MummyProjectileImage() {
    return ugdk::resource::GetTextureAtlasFromTag("mummy_projectile");
}

TextureAtlas* ImageFactory::DoorImage() {
    return ugdk::resource::GetTextureAtlasFromTag("stairs");
}

TextureAtlas* ImageFactory::WallImage() {
    return ugdk::resource::GetTextureAtlasFromTag("wall");
}

TextureAtlas* ImageFactory::EntryImage() {
    return ugdk::resource::GetTextureAtlasFromTag("door");
}

TextureAtlas* ImageFactory::LightningImage() {
    return ugdk::resource::GetTextureAtlasFromTag("lightning_bolt");
}

TextureAtlas* ImageFactory::LightImage() {
    return ugdk::resource::GetTextureAtlasFromTag("light");
}

TextureAtlas* ImageFactory::TileSwitchImage() {
    return ugdk::resource::GetTextureAtlasFromTag("tile_switch");
}

ugdk::internal::GLTexture* ImageFactory::LifePotionImage() {
    return ugdk::resource::GetTextureFromFile("images/life_potion2.png");
}

ugdk::internal::GLTexture* ImageFactory::ManaPotionImage() {
    return ugdk::resource::GetTextureFromFile("images/mana_potion.png");
}

}

