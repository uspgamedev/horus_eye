//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/imagefactory.h
// Definicao da classe ImageFactory.
//

#include "imagefactory.h"
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/image.h"

using namespace framework;
namespace utils {
    ImageFactory::ImageFactory(){
        hero_image_ = VIDEO_MANAGER()->LoadImage("data/images/sprite-sheet_MOD3.png");
        hero_image_->set_frame_size(Vector2D(110, 110));

        mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/sprite-sheet_MOD3_bw.png");
        mummy_image_->set_frame_size(Vector2D(110, 110));

        ranged_mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/sprite-sheet_MOD3_red.png");
        ranged_mummy_image_->set_frame_size(Vector2D(110, 110));

        big_mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/big_mummy_193x193.png");
        big_mummy_image_->set_frame_size(Vector2D(193, 193));

		pharaoh_image_ = VIDEO_MANAGER()->LoadImage("data/images/pharaoh_193x193.png");
        pharaoh_image_->set_frame_size(Vector2D(193, 193));

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
        floor_image_->set_frame_size(Vector2D(106,61));

        wall_image_ = VIDEO_MANAGER()->LoadImage("data/images/stoneblock3.png");
        wall_image_->set_frame_size(Vector2D(106, 157));
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
}
