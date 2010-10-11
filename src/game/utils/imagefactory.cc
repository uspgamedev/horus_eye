#include "imagefactory.h"
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/image.h"
#include "constants.h"

using namespace framework;
namespace utils {
    ImageFactory::ImageFactory(){
        hero_image_ = VIDEO_MANAGER()->LoadImage("data/images/sprite-sheet_MOD3.png");
        hero_image_->set_frame_size(Vector2D(110, 110));

        mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/mummy_blue_120x140.png");
        mummy_image_->set_frame_size(Vector2D(120, 140));

        ranged_mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/mummy_red_120x140.png");
        ranged_mummy_image_->set_frame_size(Vector2D(120, 140));

        big_mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/mummy_green_204x238.png");
        big_mummy_image_->set_frame_size(Vector2D(204, 238));

		pharaoh_image_ = VIDEO_MANAGER()->LoadImage("data/images/pharaoh_120x140.png");
        pharaoh_image_->set_frame_size(Vector2D(120, 140));

        magicmissile_image_ = VIDEO_MANAGER()->LoadImage("data/images/blue_fire_ball.png");
        magicmissile_image_->set_frame_size( Vector2D(32, 32) );

        fireball_image_ = VIDEO_MANAGER()->LoadImage("data/images/fire_ball.png");
        fireball_image_->set_frame_size( Vector2D(32, 32) );

        mummy_projectile_image_= VIDEO_MANAGER()->LoadImage("data/images/green_fire_ball.png");
        mummy_projectile_image_->set_frame_size( Vector2D(32, 32) );

        explosion_image_ = VIDEO_MANAGER()->LoadImage("data/images/explosion.png");
        explosion_image_->set_frame_size( Vector2D(256, 128) );

        quake_image_ = VIDEO_MANAGER()->LoadImage("data/images/quake.png");
        quake_image_->set_frame_size( Vector2D(256, 128) );

        life_potion_image_ = VIDEO_MANAGER()->LoadImage("data/images/life_potion2.png");
        life_potion_image_->set_frame_size(Vector2D(30, 30));

        mana_potion_image_ = VIDEO_MANAGER()->LoadImage("data/images/mana_potion.png");
        mana_potion_image_->set_frame_size(Vector2D(30, 30));

        sight_potion_image_ = VIDEO_MANAGER()->LoadImage("data/images/sight_potion.png");
        sight_potion_image_->set_frame_size(Vector2D(30, 30));

        door_image_ = VIDEO_MANAGER()->LoadImage("data/images/stairs3.png");
        door_image_->set_frame_size(Vector2D(153, 109));

        floor_image_ = VIDEO_MANAGER()->LoadImage("data/images/ground2_106x54.png");
        floor_image_->set_frame_size(Vector2D(106,54));

        wall_image_ = VIDEO_MANAGER()->LoadImage("data/images/stoneblock3.png");
        wall_image_->set_frame_size(Vector2D(106, 157));

        entry_image_ = VIDEO_MANAGER()->LoadImage("data/images/door.png");
        entry_image_->set_frame_size(Vector2D(106, 157));

        life_bar_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_life_bar.png");
        life_bar_image_->set_frame_size(Vector2D(Constants::LIFE_BAR_WIDTH, Constants::LIFE_BAR_HEIGHT));
        
        mana_bar_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_mana_bar.png");
        mana_bar_image_->set_frame_size(Vector2D(Constants::MANA_BAR_WIDTH, Constants::MANA_BAR_HEIGHT));

        totem_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_totem.png");
        totem_image_->set_frame_size(Vector2D(72, 518));

        eye_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_eye.png");
        eye_image_->set_frame_size(Vector2D(200, 80));
        
        back_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_horizontal_barx10.png");
        back_image_->set_frame_size(Vector2D(1150, 80));

        mummy_counter_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_mummy_counter.png");
        mummy_counter_image_->set_frame_size(Vector2D(190, 138));

        earthquake_icon_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_magic_rock.png");
        earthquake_icon_image_->set_frame_size(Vector2D(25, 35));
        
        fireball_icon_image_ = VIDEO_MANAGER()->LoadImage("data/images/hud_magic_fire.png");
        fireball_icon_image_->set_frame_size(Vector2D(25, 35));
    }
    
    Image* ImageFactory::FireballIconImage() {
        return fireball_icon_image_;
    }
    Image* ImageFactory::EarthquakeIconImage() {
        return earthquake_icon_image_;
    }
    Image* ImageFactory::HeroImage(){
        return hero_image_;
    }
    Image* ImageFactory::BigMummyImage(){
        return big_mummy_image_;
    }
    Image* ImageFactory::PharaohImage(){
        return pharaoh_image_;
    }
    Image* ImageFactory::MummyImage(){
        return mummy_image_;
    }
    Image* ImageFactory::RangedMummyImage(){
        return ranged_mummy_image_;
    }
    Image* ImageFactory::MagicMissileImage() {
        return magicmissile_image_;
    }
    Image* ImageFactory::FireballImage() {
        return fireball_image_;
    }
    Image* ImageFactory::MummyProjectileImage() {
        return mummy_projectile_image_;
    }
    Image* ImageFactory::ExplosionImage() {
        return explosion_image_;
    }
    Image* ImageFactory::QuakeImage() {
        return quake_image_;
    }
    Image* ImageFactory::LifeBarImage() {
        return life_bar_image_;
    }
    Image* ImageFactory::ManaBarImage() {
        return mana_bar_image_;
    }
    Image* ImageFactory::TotemImage() {
        return totem_image_;
    }
    Image* ImageFactory::LifePotionImage() {
        return life_potion_image_;
    }
    Image* ImageFactory::ManaPotionImage() {
        return mana_potion_image_;
    }
    Image* ImageFactory::SightPotionImage() {
        return sight_potion_image_;
    }
    Image* ImageFactory::DoorImage() {
        return door_image_;
    }
    Image* ImageFactory::FloorImage() {
        return floor_image_;
    }
    Image* ImageFactory::WallImage() {
        return wall_image_;
    }
    Image* ImageFactory::EntryImage() {
        return entry_image_;
    }
    Image* ImageFactory::BackImage() {
        return back_image_;
    }
    Image* ImageFactory::EyeImage() {
        return eye_image_;
    }
    Image* ImageFactory::MummyCounterImage() {
        return mummy_counter_image_;
    }
}

