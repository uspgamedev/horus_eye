#include "imagefactory.h"
#include "game/config.h"
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/image.h>
#include "constants.h"

using namespace ugdk;
namespace utils {
  
Image* ImageFactory::HeroImage() {
    Image *hero_image_ = VIDEO_MANAGER()->LoadImage("data/images/sprite-sheet_MOD3.png");
    if(hero_image_) hero_image_->set_frame_size(Vector2D(110, 110));
    return hero_image_;
}

Image* ImageFactory::MummyImage(){
    Image *mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/mummy_blue_120x140.png");
    if(mummy_image_) mummy_image_->set_frame_size(Vector2D(120, 140));
    return mummy_image_;
}

Image* ImageFactory::BigMummyImage() {
    Image *big_mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/mummy_green_120x140.png");
	if(big_mummy_image_) {
		big_mummy_image_->set_frame_size(Vector2D(120, 140));
		big_mummy_image_->set_render_size(Vector2D(204, 238));
	}
    return big_mummy_image_;
}

Image* ImageFactory::PharaohImage(){
    Image *pharaoh_image_ = VIDEO_MANAGER()->LoadImage("data/images/pharaoh_120x140.png");
    if(pharaoh_image_) pharaoh_image_->set_frame_size(Vector2D(120, 140));
    return pharaoh_image_;
}

Image* ImageFactory::RangedMummyImage(){
    Image *ranged_mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/mummy_red_120x140.png");
    if(ranged_mummy_image_) ranged_mummy_image_->set_frame_size(Vector2D(120, 140));
    return ranged_mummy_image_;
}

Image* ImageFactory::MagicMissileImage() {
    Image *magicmissile_image_ = VIDEO_MANAGER()->LoadImage("data/images/blue_fire_ball.png");
    if(magicmissile_image_) magicmissile_image_->set_frame_size( Vector2D(32, 32) );
    return magicmissile_image_;
}

Image* ImageFactory::FireballImage() {
    Image *fireball_image_ = VIDEO_MANAGER()->LoadImage("data/images/fireball_0.png");
    if(fireball_image_) fireball_image_->set_frame_size( Vector2D(64, 64) );
    return fireball_image_;
}

Image* ImageFactory::MummyProjectileImage() {
    Image *mummy_projectile_image_ = VIDEO_MANAGER()->LoadImage("data/images/green_fire_ball.png");
    if(mummy_projectile_image_) mummy_projectile_image_->set_frame_size( Vector2D(32, 32) );
    return mummy_projectile_image_;
}

Image* ImageFactory::ExplosionImage() {
    Image *explosion_image_ = VIDEO_MANAGER()->LoadImage("data/images/explosion.png");
    if(explosion_image_) explosion_image_->set_frame_size( Vector2D(256, 128) );
    return explosion_image_;
}

Image* ImageFactory::QuakeImage() {
    Image *quake_image_ = VIDEO_MANAGER()->LoadImage("data/images/quake.png");
    if(quake_image_) quake_image_->set_frame_size( Vector2D(256, 128) );
    return quake_image_;
}

Image* ImageFactory::LifePotionImage() {
    Image *life_potion_image_ = VIDEO_MANAGER()->LoadImage("data/images/life_potion2.png");
    if(life_potion_image_) life_potion_image_->set_frame_size(Vector2D(30, 30));
    return life_potion_image_;
}

Image* ImageFactory::ManaPotionImage() {
    Image *mana_potion_image_ = VIDEO_MANAGER()->LoadImage("data/images/mana_potion.png");
    if(mana_potion_image_) mana_potion_image_->set_frame_size(Vector2D(30, 30));
    return mana_potion_image_;
}

Image* ImageFactory::SightPotionImage() {
    Image *sight_potion_image_ = VIDEO_MANAGER()->LoadImage("data/images/sight_potion.png");
    if(sight_potion_image_) sight_potion_image_->set_frame_size(Vector2D(30, 30));
    return sight_potion_image_;
}

Image* ImageFactory::DoorImage() {
    Image *door_image_ = VIDEO_MANAGER()->LoadImage("data/images/stairs3.png");
    if(door_image_) door_image_->set_frame_size(Vector2D(153, 109));
    return door_image_;
}

Image* ImageFactory::FloorImage() {
    Image *floor_image_ = VIDEO_MANAGER()->LoadImage("data/images/ground2_106x54.png");
    if(floor_image_) floor_image_->set_frame_size(Vector2D(106,54));
    return floor_image_;
}

Image* ImageFactory::WallImage() {
    Image *wall_image_ = VIDEO_MANAGER()->LoadImage("data/images/stoneblock3.png");
    if(wall_image_) wall_image_->set_frame_size(Vector2D(106, 157));
    return wall_image_;
}

Image* ImageFactory::EntryImage() {
    Image *entry_image_ = VIDEO_MANAGER()->LoadImage("data/images/door.png");
    if(entry_image_) entry_image_->set_frame_size(Vector2D(106, 157));
    return entry_image_;
}

Image* ImageFactory::LightningImage() {
    Image *lightning_image_ = VIDEO_MANAGER()->LoadImage("data/images/lightning_bolt.png");
    if(lightning_image_) lightning_image_->set_frame_size(Vector2D(64, 64));
    return lightning_image_;
}

Image* ImageFactory::LightImage() {
    Image *light_image_ = VIDEO_MANAGER()->LoadImage("data/images/yellow_fire_ball.png");
    if(light_image_) light_image_->set_frame_size( Vector2D(32, 32) );
    return light_image_;
}

Image* ImageFactory::TileSwitchImage() {
    Image *floor_image_ = VIDEO_MANAGER()->LoadImage("data/images/tile_switch.png");
    if(floor_image_) floor_image_->set_frame_size(Vector2D(106,54));
    return floor_image_;
}

}

