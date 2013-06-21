#include "imagefactory.h"
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include "game/constants.h"

using ugdk::base::ResourceManager;
using ugdk::math::Vector2D;
using ugdk::graphic::Drawable;
using ugdk::graphic::Spritesheet;

namespace utils {

Spritesheet* ImageFactory::HeroImage() {
    return ResourceManager::GetSpritesheetFromTag("hero");
}

Spritesheet* ImageFactory::MummyImage(){
    return ResourceManager::GetSpritesheetFromTag("mummy_basic");
}

Spritesheet* ImageFactory::BigMummyImage() {
    return ResourceManager::GetSpritesheetFromTag("mummy_big");
}

Spritesheet* ImageFactory::PharaohImage(){
    return ResourceManager::GetSpritesheetFromTag("pharaoh");
}

Spritesheet* ImageFactory::RangedMummyImage(){
    return ResourceManager::GetSpritesheetFromTag("mummy_ranged");
}

Spritesheet* ImageFactory::MagicMissileImage() {
    return ResourceManager::GetSpritesheetFromTag("magic_missile");
}

Spritesheet* ImageFactory::FireballImage() {
    return ResourceManager::GetSpritesheetFromTag("fireball");
}

Spritesheet* ImageFactory::MummyProjectileImage() {
    return ResourceManager::GetSpritesheetFromTag("mummy_projectile");
}

Spritesheet* ImageFactory::DoorImage() {
    return ResourceManager::GetSpritesheetFromTag("stairs");
}

Spritesheet* ImageFactory::WallImage() {
    return ResourceManager::GetSpritesheetFromTag("wall");
}

Spritesheet* ImageFactory::EntryImage() {
    return ResourceManager::GetSpritesheetFromTag("door");
}

Spritesheet* ImageFactory::LightningImage() {
    return ResourceManager::GetSpritesheetFromTag("lightning_bolt");
}

Spritesheet* ImageFactory::LightImage() {
    return ResourceManager::GetSpritesheetFromTag("light");
}

Spritesheet* ImageFactory::TileSwitchImage() {
    return ResourceManager::GetSpritesheetFromTag("tile_switch");
}

Drawable* ImageFactory::LifePotionImage() {
    return new ugdk::graphic::TexturedRectangle(ugdk::base::ResourceManager::GetTextureFromFile("images/life_potion2.png"));
}

Drawable* ImageFactory::ManaPotionImage() {
    return new ugdk::graphic::TexturedRectangle(ugdk::base::ResourceManager::GetTextureFromFile("images/mana_potion.png"));
}

}

