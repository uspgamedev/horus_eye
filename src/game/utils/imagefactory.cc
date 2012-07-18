#include "imagefactory.h"
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include "constants.h"

using namespace ugdk;
using ugdk::base::ResourceManager;
using ugdk::graphic::Drawable;
using ugdk::graphic::Spritesheet;

namespace utils {

#define HERO_HOTSPOT_X 55.0
#define HERO_HOTSPOT_Y 102.0
  
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
    return ResourceManager::GetSpritesheetFromTag("images/fireball_0.png");
}

Spritesheet* ImageFactory::MummyProjectileImage() {
    return ResourceManager::GetSpritesheetFromTag("images/green_fire_ball.png");
}

Spritesheet* ImageFactory::ExplosionImage() {
    return ResourceManager::GetSpritesheetFromTag("images/explosion.png");
}

Spritesheet* ImageFactory::QuakeImage() {
    return ResourceManager::GetSpritesheetFromTag("images/quake.png");
}

Drawable* ImageFactory::LifePotionImage() {
    return new graphic::TexturedRectangle(ugdk::base::ResourceManager::GetTextureFromFile("images/life_potion2.png"));
}

Drawable* ImageFactory::ManaPotionImage() {
    return new graphic::TexturedRectangle(ugdk::base::ResourceManager::GetTextureFromFile("images/mana_potion.png"));
}

Drawable* ImageFactory::BlueGemImage() {
    return new graphic::Sprite(ResourceManager::GetSpritesheetFromTag("images/yellow_fire_ball.png"));
}

Spritesheet* ImageFactory::ShieldImage() {
    return ResourceManager::GetSpritesheetFromTag("images/shield.png");
}

Drawable* ImageFactory::SightPotionImage() {
    return new graphic::TexturedRectangle(ugdk::base::ResourceManager::GetTextureFromFile("images/sight_potion.png"));
}

Spritesheet* ImageFactory::DoorImage() {
    return ResourceManager::GetSpritesheetFromTag("images/stairs3.png");
}

Drawable* ImageFactory::FloorImage() {
    Drawable* draw = new graphic::TexturedRectangle(ugdk::base::ResourceManager::GetTextureFromFile("images/ground2_106x54.png"));
    draw->set_hotspot(Vector2D(Constants::FLOOR_HOTSPOT_X, Constants::FLOOR_HOTSPOT_Y));
    return draw;
}

Spritesheet* ImageFactory::WallImage() {
    return ResourceManager::GetSpritesheetFromTag("images/stoneblock3.png");
}

Spritesheet* ImageFactory::EntryImage() {
    return ResourceManager::GetSpritesheetFromTag("images/door.png");
}

Spritesheet* ImageFactory::LightningImage() {
    return ResourceManager::GetSpritesheetFromTag("images/lightning_bolt.png");
}

Spritesheet* ImageFactory::LightImage() {
    return ResourceManager::GetSpritesheetFromTag("images/yellow_fire_ball.png");
}

Spritesheet* ImageFactory::TileSwitchImage() {
    return ResourceManager::GetSpritesheetFromTag("images/tile_switch.png");
}

}

