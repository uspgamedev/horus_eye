#include "imagefactory.h"
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include "constants.h"

using namespace ugdk;
namespace utils {

#define HERO_HOTSPOT_X 55.0
#define HERO_HOTSPOT_Y 102.0
  
ugdk::graphic::Spritesheet* ImageFactory::HeroImage() {
    return RESOURCE_MANAGER()->spritesheet_container().Find("images/sprite-sheet_MOD3.png");
}

FlexibleSpritesheet* ImageFactory::MummyImage(){
    return static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/mummy_blue_120x140.png"));
}

FlexibleSpritesheet* ImageFactory::BigMummyImage() {
    return static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/mummy_green_120x140.png"));
}

FlexibleSpritesheet* ImageFactory::PharaohImage(){
    return static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/pharaoh_120x140.png"));
}

FlexibleSpritesheet* ImageFactory::RangedMummyImage(){
    return static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/mummy_red_120x140.png"));
}

FlexibleSpritesheet* ImageFactory::MagicMissileImage() {
    return static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/blue_fire_ball.png"));
}

FlexibleSpritesheet* ImageFactory::FireballImage() {
    FlexibleSpritesheet *fireball_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/fireball_0.png"));
    if(fireball_image_) fireball_image_->set_frame_size( Vector2D(64, 64) );
    return fireball_image_;
}

FlexibleSpritesheet* ImageFactory::MummyProjectileImage() {
    FlexibleSpritesheet *mummy_projectile_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/green_fire_ball.png"));
    if(mummy_projectile_image_) mummy_projectile_image_->set_frame_size( Vector2D(32, 32) );
    return mummy_projectile_image_;
}

FlexibleSpritesheet* ImageFactory::ExplosionImage() {
    FlexibleSpritesheet *explosion_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/explosion.png"));
    if(explosion_image_) explosion_image_->set_frame_size( Vector2D(256, 128) );
    return explosion_image_;
}

FlexibleSpritesheet* ImageFactory::QuakeImage() {
    FlexibleSpritesheet *quake_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/quake.png"));
    if(quake_image_) quake_image_->set_frame_size( Vector2D(256, 128) );
    return quake_image_;
}

ugdk::graphic::Drawable* ImageFactory::LifePotionImage() {
    return new graphic::TexturedRectangle(ugdk::base::ResourceManager::GetTextureFromFile("images/life_potion2.png"));
}

ugdk::graphic::Drawable* ImageFactory::ManaPotionImage() {
    return new graphic::TexturedRectangle(ugdk::base::ResourceManager::GetTextureFromFile("images/mana_potion.png"));
}

ugdk::graphic::Drawable* ImageFactory::BlueGemImage() {
    return new graphic::Sprite(RESOURCE_MANAGER()->spritesheet_container().Find("images/yellow_fire_ball.png"));
}

ugdk::graphic::Spritesheet* ImageFactory::ShieldImage() {
    return RESOURCE_MANAGER()->spritesheet_container().Find("images/shield.png");
}

ugdk::graphic::Drawable* ImageFactory::SightPotionImage() {
    return new graphic::TexturedRectangle(ugdk::base::ResourceManager::GetTextureFromFile("images/sight_potion.png"));
}

FlexibleSpritesheet* ImageFactory::DoorImage() {
    FlexibleSpritesheet *door_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/stairs3.png"));
    if(door_image_) door_image_->set_frame_size(Vector2D(153, 109));
    return door_image_;
}

FlexibleSpritesheet* ImageFactory::FloorImage() {
    FlexibleSpritesheet *floor_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/ground2_106x54.png"));
    if(floor_image_) floor_image_->set_frame_size(Vector2D(106,54));
    return floor_image_;
}

FlexibleSpritesheet* ImageFactory::WallImage() {
    FlexibleSpritesheet *wall_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/stoneblock3.png"));
    if(wall_image_) wall_image_->set_frame_size(Vector2D(106, 157));
    return wall_image_;
}

FlexibleSpritesheet* ImageFactory::EntryImage() {
    FlexibleSpritesheet *entry_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/door.png"));
    if(entry_image_) entry_image_->set_frame_size(Vector2D(106, 157));
    return entry_image_;
}

FlexibleSpritesheet* ImageFactory::LightningImage() {
    FlexibleSpritesheet *lightning_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/lightning_bolt.png"));
    if(lightning_image_) lightning_image_->set_frame_size(Vector2D(64, 64));
    return lightning_image_;
}

FlexibleSpritesheet* ImageFactory::LightImage() {
    FlexibleSpritesheet *light_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/yellow_fire_ball.png"));
    if(light_image_) light_image_->set_frame_size( Vector2D(32, 32) );
    return light_image_;
}

FlexibleSpritesheet* ImageFactory::TileSwitchImage() {
    FlexibleSpritesheet *floor_image_ = static_cast<FlexibleSpritesheet*>(RESOURCE_MANAGER()->spritesheet_container().Find("images/tile_switch.png"));
    if(floor_image_) floor_image_->set_frame_size(Vector2D(106,54));
    return floor_image_;
}

}

