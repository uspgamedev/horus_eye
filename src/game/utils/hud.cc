#include "hud.h"
#include "../../framework/vector2D.h"
#include "../../framework/sprite.h"
#include "../../framework/engine.h"
#include "../../framework/animation.h"
#include "../../framework/videomanager.h"
#include "../scenes/world.h"
#include "../utils/hudimagefactory.h"
#include "constants.h"
#include "../sprites/weapons/weapon.h"

#define LIFE_IMAGE_WIDTH Constants::LIFE_IMAGE_WIDTH
#define LIFE_IMAGE_HEIGHT Constants::LIFE_IMAGE_HEIGHT

#define LIFE_BAR_OFFSET_X Constants::LIFE_BAR_OFFSET_X
#define LIFE_BAR_OFFSET_Y Constants::LIFE_BAR_OFFSET_Y
#define LIFE_BAR_WIDTH Constants::LIFE_BAR_WIDTH
#define LIFE_BAR_HEIGHT Constants::LIFE_BAR_HEIGHT

#define MANA_BAR_OFFSET_X Constants::MANA_BAR_OFFSET_X
#define MANA_BAR_OFFSET_Y Constants::MANA_BAR_OFFSET_Y
#define MANA_BAR_WIDTH Constants::MANA_BAR_WIDTH
#define MANA_BAR_HEIGHT Constants::MANA_BAR_HEIGHT

#define TOTEM_WIDTH Constants::TOTEM_WIDTH
#define TOTEM_HEIGHT Constants::TOTEM_HEIGHT
#define TOTEM_OFFSET_X Constants::TOTEM_OFFSET_X
#define TOTEM_OFFSET_Y Constants::TOTEM_OFFSET_Y

#define ENEMY_COUNTER_OFFSET_X -10
#define ENEMY_COUNTER_OFFSET_Y  30

#define FPS_BAR_OFFSET_X  10
#define FPS_BAR_OFFSET_Y  30

#define VIDEO_Y VIDEO_MANAGER()->video_size().y 
#define VIDEO_X VIDEO_MANAGER()->video_size().x 

#define NUMBER_WIDTH 18
#define NUMBER_HEIGHT 16

using namespace framework;
using namespace scene;

namespace utils {

// Aviso: maximo de 999 mumias no display de inimigos restantes!
Hud::Hud(World* world) {
    Image* number = VIDEO_MANAGER()->LoadImage("data/images/numbers2.png");
    number->set_frame_size(Vector2D(NUMBER_WIDTH, NUMBER_HEIGHT));

    Image* life_bar_image_;
    Image* mana_bar_image_;
    Image* totem_image_;
    Image* back_image;
    Image* eye_image;
     
    
    //Criando sprites da life bar
    HudImageFactory img_fac;

    life_bar_image_ = img_fac.LifeBarImage();
    life_bar_ = new Sprite;
    life_bar_->Initialize(life_bar_image_);
    life_bar_->set_hotspot(Vector2D(LIFE_BAR_WIDTH/2, LIFE_BAR_HEIGHT));
    life_bar_->set_position(Vector2D(VIDEO_X - LIFE_BAR_OFFSET_X, VIDEO_Y - LIFE_BAR_OFFSET_Y));
    AddSprite(life_bar_);
    
    
    mana_bar_image_ = img_fac.ManaBarImage();
    mana_bar_ = new Sprite;
    mana_bar_->Initialize(mana_bar_image_);
    mana_bar_->set_hotspot(Vector2D(MANA_BAR_WIDTH/2, MANA_BAR_HEIGHT));
    mana_bar_->set_position(Vector2D(MANA_BAR_OFFSET_X , VIDEO_Y - MANA_BAR_OFFSET_Y));
    AddSprite(mana_bar_);

    back_image = img_fac.BackImage();
    Sprite *backLeft = new Sprite();
    backLeft->Initialize(back_image);
    if(back_image) backLeft->set_hotspot(Vector2D(back_image->width(), back_image->height()));
    Sprite *backRight = new Sprite();
    backRight->Initialize(back_image);
    if(back_image) backRight->set_hotspot(Vector2D(0, back_image->height()));

    eye_image = img_fac.EyeImage();
    Sprite *eye = new Sprite();
    eye->Initialize(eye_image);
    if(eye_image) eye->set_hotspot(Vector2D(eye_image->width()/2, eye_image->height()));

    Image *mummy_counter_image = img_fac.MummyCounterImage();
    Sprite *mummy_counter = new Sprite();
    mummy_counter->Initialize(mummy_counter_image);
    if(mummy_counter_image) mummy_counter->set_hotspot(Vector2D(0, mummy_counter_image->height()));

    weapon_icon_ = NULL;
    
    // setando posicoes
    eye->set_position(Vector2D(VIDEO_X/2, VIDEO_Y));
    backLeft->set_position(Vector2D(VIDEO_X/2 - (eye_image ? eye_image->width()/2 : 0), VIDEO_Y));
    backRight->set_position(Vector2D(VIDEO_X/2 + (eye_image ? eye_image->width()/2 : 0), VIDEO_Y));
    mummy_counter->set_position(Vector2D(VIDEO_X/2 + (eye_image ? eye_image->width()/2 : 0), VIDEO_Y));

    AddSprite(backLeft);
    AddSprite(backRight);
    AddSprite(eye);
    AddSprite(mummy_counter);

    totem_image_ = img_fac.TotemImage();
    for (int i = 0; i < 2; i++) {
        totem_[i] = new Sprite;
        totem_[i]->Initialize(totem_image_);
        totem_[i]->set_hotspot(Vector2D(TOTEM_WIDTH/2, TOTEM_HEIGHT));
        if (i == 0) totem_[i]->set_position(Vector2D(TOTEM_OFFSET_X , VIDEO_Y - TOTEM_OFFSET_Y));
        if (i == 1) totem_[i]->set_position(Vector2D(VIDEO_X - TOTEM_OFFSET_X , VIDEO_Y - TOTEM_OFFSET_Y));
        totem_[i]->set_zindex(-1.0f);
        AddSprite(totem_[i]);
    }

    for(int i = 0; i < 3; ++i) {
        enemy_counter_[i] = new Sprite;
        enemy_counter_[i]->Initialize(number);
            
        enemy_counter_[i]->set_position(Vector2D(
                VIDEO_X/2 + (eye_image ? eye_image->width()/1.1 : 0) - NUMBER_WIDTH*(i+1),
                VIDEO_Y - (back_image ? back_image->height()/2 : 0) ));
        AddSprite(enemy_counter_[i]);
        enemy_counter_value_[i] = 0;
    }

    for(int i = 0; i < 3; ++i) {
        (fps_meter_[i] = new Sprite)->Initialize(number);
        fps_meter_[i]->set_position(Vector2D(
                        FPS_BAR_OFFSET_X + NUMBER_WIDTH*i + 0.0f, FPS_BAR_OFFSET_Y + 0.0f));
#ifdef DEBUG
        AddSprite(fps_meter_[i]);
#endif
        fps_meter_value_[i] = 0;
    }
}

Hud::~Hud() {
    
}

void Hud::Update(float delta_t) {
    Layer::Update(delta_t);
    World* world = WORLD();

    int digit[7], enemy_number;
    enemy_number = (enemy_number = world->CountRemainingEnemies()) > 999 ? 999 : enemy_number;
    digit[2] = enemy_number / 100;
    digit[1] = (enemy_number / 10) % 10;
    digit[0] = enemy_number % 10;

    // TODO: fixme (Contadores)
    /*
    for(int i = 0; i < 3; ++i) {
        if(digit[i] != enemy_counter_value_[i]) {
            enemy_counter_value_[i] = digit[i];
            enemy_counter_[i]->animation()->set_framelist(enemy_counter_value_[i], -1);
        }
    }

    int fps = Engine::reference()->current_fps();
    if(fps > 999) fps = 999;
    for(int i = 2; i >= 0; --i) {
        digit[i] = fps % 10;
        fps /= 10;
    }
    for(int i = 0; i < 3; ++i) {
        if(digit[i] != fps_meter_value_[i]) {
            fps_meter_value_[i] = digit[i];
            fps_meter_[i]->animation()->set_framelist(fps_meter_value_[i], -1);
        }
    } */

    if (weapon_icon_ != NULL) {
        icon_added[weapon_icon_]->set_visible(false);
    }

    if(world->hero() != NULL) {
        if(world->hero()->secondary_weapon() != NULL)
            weapon_icon_ = world->hero()->secondary_weapon()->icon();

        if (world->hero()->life() > 0) {
            life_bar_->set_visible(true);
            float new_height = ((float)world->hero()->life() * LIFE_BAR_HEIGHT) / ((float)world->hero()->max_life());
            //if(life_bar_->image()) life_bar_->image()->set_frame_size(Vector2D(LIFE_BAR_WIDTH, new_height));
            life_bar_->set_hotspot(Vector2D(LIFE_BAR_WIDTH/2, new_height));
        } else {
            life_bar_->set_visible(false);
        }
        if (world->hero()->mana() > 0) {
            mana_bar_->set_visible(true);
            float new_height = ((float)world->hero()->mana() * MANA_BAR_HEIGHT) / ((float)world->hero()->max_mana());
            //if(mana_bar_->image()) mana_bar_->image()->set_frame_size(Vector2D(MANA_BAR_WIDTH, new_height));
            mana_bar_->set_hotspot(Vector2D(MANA_BAR_WIDTH/2, new_height));
        } else {
            mana_bar_->set_visible(false);
        }
    }

    if (weapon_icon_ != NULL && icon_added[weapon_icon_] == NULL) {
        Sprite* s = new Sprite;
        s->Initialize(weapon_icon_);

        s->set_hotspot(Vector2D(weapon_icon_->width()/2, weapon_icon_->height()/2));
        s->set_position(Vector2D(VIDEO_X/2 - 5, VIDEO_Y - 47));
        
        AddSprite(s);
        icon_added[weapon_icon_] = s;
    }
    if(weapon_icon_)
        icon_added[weapon_icon_]->set_visible(true);

}

}
