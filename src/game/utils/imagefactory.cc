#include "imagefactory.h"
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include "constants.h"

using namespace ugdk;
namespace utils {

#define HERO_HOTSPOT_X 55.0f
#define HERO_HOTSPOT_Y 102.0f
  
Spritesheet* ImageFactory::HeroImage() {
    return VIDEO_MANAGER()->GetSpritesheet("data/images/sprite-sheet_MOD3.png");
}

FlexibleSpritesheet* ImageFactory::MummyImage(){
    FlexibleSpritesheet *mummy_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/mummy_blue_120x140.png"));
    if(mummy_image_) {
        mummy_image_->set_frame_size(Vector2D(120.0f, 140.0f));
        mummy_image_->set_hotspot(Vector2D(60.0f, 120.0f));
    }
    return mummy_image_;
}

FlexibleSpritesheet* ImageFactory::BigMummyImage() {
    FlexibleSpritesheet *big_mummy_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/mummy_green_120x140.png"));
	if(big_mummy_image_) {
		big_mummy_image_->set_frame_size(Vector2D(120, 140));
        big_mummy_image_->set_hotspot(Vector2D(60.0f, 120.0f));
	}
    return big_mummy_image_;
}

FlexibleSpritesheet* ImageFactory::PharaohImage(){
    FlexibleSpritesheet *pharaoh_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/pharaoh_120x140.png"));
    if(pharaoh_image_) {
        pharaoh_image_->set_frame_size(Vector2D(120, 140));
        pharaoh_image_->set_hotspot(Vector2D(60.0f, 120.0f));
    }
    return pharaoh_image_;
}

FlexibleSpritesheet* ImageFactory::RangedMummyImage(){
    FlexibleSpritesheet *ranged_mummy_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/mummy_red_120x140.png"));
    if(ranged_mummy_image_) {
        ranged_mummy_image_->set_frame_size(Vector2D(120, 140));
        ranged_mummy_image_->set_hotspot(Vector2D(60.0f, 120.0f));
    }
    return ranged_mummy_image_;
}

FlexibleSpritesheet* ImageFactory::MagicMissileImage() {
    FlexibleSpritesheet *magicmissile_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/blue_fire_ball.png"));
    if(magicmissile_image_) magicmissile_image_->set_frame_size( Vector2D(32, 32) );
    return magicmissile_image_;
}

FlexibleSpritesheet* ImageFactory::FireballImage() {
    FlexibleSpritesheet *fireball_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/fireball_0.png"));
    if(fireball_image_) fireball_image_->set_frame_size( Vector2D(64, 64) );
    return fireball_image_;
}

FlexibleSpritesheet* ImageFactory::MummyProjectileImage() {
    FlexibleSpritesheet *mummy_projectile_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/green_fire_ball.png"));
    if(mummy_projectile_image_) mummy_projectile_image_->set_frame_size( Vector2D(32, 32) );
    return mummy_projectile_image_;
}

FlexibleSpritesheet* ImageFactory::ExplosionImage() {
    FlexibleSpritesheet *explosion_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/explosion.png"));
    if(explosion_image_) explosion_image_->set_frame_size( Vector2D(256, 128) );
    return explosion_image_;
}

FlexibleSpritesheet* ImageFactory::QuakeImage() {
    FlexibleSpritesheet *quake_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/quake.png"));
    if(quake_image_) quake_image_->set_frame_size( Vector2D(256, 128) );
    return quake_image_;
}

FlexibleSpritesheet* ImageFactory::LifePotionImage() {
    FlexibleSpritesheet *life_potion_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/life_potion2.png"));
    if(life_potion_image_) life_potion_image_->set_frame_size(Vector2D(30, 30));
    return life_potion_image_;
}

FlexibleSpritesheet* ImageFactory::ManaPotionImage() {
    FlexibleSpritesheet *mana_potion_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/mana_potion.png"));
    if(mana_potion_image_) mana_potion_image_->set_frame_size(Vector2D(30, 30));
    return mana_potion_image_;
}

FlexibleSpritesheet* ImageFactory::SightPotionImage() {
    FlexibleSpritesheet *sight_potion_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/sight_potion.png"));
    if(sight_potion_image_) sight_potion_image_->set_frame_size(Vector2D(30, 30));
    return sight_potion_image_;
}

FlexibleSpritesheet* ImageFactory::DoorImage() {
    FlexibleSpritesheet *door_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/stairs3.png"));
    if(door_image_) door_image_->set_frame_size(Vector2D(153, 109));
    return door_image_;
}

FlexibleSpritesheet* ImageFactory::FloorImage() {
    FlexibleSpritesheet *floor_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/ground2_106x54.png"));
    if(floor_image_) floor_image_->set_frame_size(Vector2D(106,54));
    return floor_image_;
}

FlexibleSpritesheet* ImageFactory::WallImage() {
    FlexibleSpritesheet *wall_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/stoneblock3.png"));
    if(wall_image_) wall_image_->set_frame_size(Vector2D(106, 157));
    return wall_image_;
}

FlexibleSpritesheet* ImageFactory::EntryImage() {
    FlexibleSpritesheet *entry_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/door.png"));
    if(entry_image_) entry_image_->set_frame_size(Vector2D(106, 157));
    return entry_image_;
}

FlexibleSpritesheet* ImageFactory::LightningImage() {
    FlexibleSpritesheet *lightning_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/lightning_bolt.png"));
    if(lightning_image_) lightning_image_->set_frame_size(Vector2D(64, 64));
    return lightning_image_;
}

FlexibleSpritesheet* ImageFactory::LightImage() {
    FlexibleSpritesheet *light_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/yellow_fire_ball.png"));
    if(light_image_) light_image_->set_frame_size( Vector2D(32, 32) );
    return light_image_;
}

FlexibleSpritesheet* ImageFactory::TileSwitchImage() {
    FlexibleSpritesheet *floor_image_ = static_cast<FlexibleSpritesheet*>(VIDEO_MANAGER()->GetSpritesheet("data/images/tile_switch.png"));
    if(floor_image_) floor_image_->set_frame_size(Vector2D(106,54));
    return floor_image_;
}

}

