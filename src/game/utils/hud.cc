#include "hud.h"
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/text.h>
#include <ugdk/base/engine.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/modifier.h>
#include "game/scenes/world.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"

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

using namespace ugdk;
using namespace scene;

namespace utils {

// Aviso: maximo de 999 mumias no display de inimigos restantes!
Hud::Hud(World* world) : node_(new Node) {
    /*Image* number = VIDEO_MANAGER()->LoadImage("data/images/numbers2.png");
    number->set_frame_size(Vector2D(NUMBER_WIDTH, NUMBER_HEIGHT));

    // Criando sprites da life bar
    HudImageFactory img_fac;

    Image* back_image = img_fac.BackImage();
    Image* eye_image = img_fac.EyeImage();

    Node* back_node = new Node(back_image);
    back_node->modifier()->set_offset(Vector2D(0.0f, VIDEO_Y - back_image->height()));

    node_->AddChild(back_node);*/

    /*Node* life_bar = new Node(img_fac.LifeBarImage(), life_modifier_ = new Modifier);
    life_bar->set_zindex(-0.5f);
    life_modifier_->set_offset(Vector2D(LIFE_BAR_OFFSET_X - LIFE_BAR_WIDTH/2, VIDEO_Y - LIFE_BAR_OFFSET_Y));
    ////AddSprite(life_bar);
    
    Node *mana_bar = new Node(img_fac.ManaBarImage(), mana_modifier_ = new Modifier);
    mana_bar->set_zindex(-0.5f);
    mana_modifier_->set_offset(Vector2D(VIDEO_X - MANA_BAR_OFFSET_X - MANA_BAR_WIDTH/2, VIDEO_Y - MANA_BAR_OFFSET_Y));
    ////AddSprite(mana_bar);

    Node *block_bar = new Node(img_fac.ManaBarImage(), block_modifier_ = new Modifier);
    block_bar->set_zindex(-0.6f);
    block_modifier_->set_offset(Vector2D(VIDEO_X - MANA_BAR_OFFSET_X - MANA_BAR_WIDTH/2, VIDEO_Y - MANA_BAR_OFFSET_Y));
    block_modifier_->set_color(ugdk::Color(0.5f, 0.5f, 0.5f));
    block_modifier_->set_alpha(0.75f);
    ////AddSprite(block_bar);*/

    /*Node *backLeft = new Node(back_image);
    backLeft->Initialize(back_image);
    if(back_image) backLeft->set_hotspot(Vector2D(back_image->width(), back_image->height()));
    Sprite *backRight = new Sprite();
    backRight->Initialize(back_image);
    if(back_image) backRight->set_hotspot(Vector2D(0, back_image->height()));

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
    eye->set_zindex(0.5f);
    backLeft->set_position(Vector2D(VIDEO_X/2 - (eye_image ? eye_image->width()/2 : 0), VIDEO_Y));
    backRight->set_position(Vector2D(VIDEO_X/2 + (eye_image ? eye_image->width()/2 : 0), VIDEO_Y));
    mummy_counter->set_position(Vector2D(VIDEO_X/2 + (eye_image ? eye_image->width()/2 : 0), VIDEO_Y));
    mummy_counter->set_zindex(0.5f);

    //AddSprite(backLeft);
    //AddSprite(backRight);
    //AddSprite(eye);
    //AddSprite(mummy_counter);

    Image* totem_image_ = img_fac.TotemImage(),
        *totem_bottom_image_ = img_fac.TotemBottomImage();

    if(totem_image_ && totem_bottom_image_)
        for (int i = 0; i < 2; i++) {
            Sprite *totem = new Sprite;
            totem->Initialize(totem_image_);
            totem->set_hotspot(totem_image_->width() * 0.5f, totem_image_->height() + 0.0f);

            Sprite *totem_bottom = new Sprite;
            totem_bottom->Initialize(totem_bottom_image_);
            totem_bottom->set_hotspot(totem_bottom_image_->width() * 0.5f, totem_bottom_image_->height());

            if (i == 0) {
                totem->set_position(TOTEM_OFFSET_X , VIDEO_Y - TOTEM_OFFSET_Y - totem_bottom_image_->height());
                totem_bottom->set_position(TOTEM_OFFSET_X, VIDEO_Y - TOTEM_OFFSET_Y);
            }
            if (i == 1) {
                totem->set_position(VIDEO_X - TOTEM_OFFSET_X , VIDEO_Y - TOTEM_OFFSET_Y - totem_bottom_image_->height());
                totem_bottom->set_position(VIDEO_X - TOTEM_OFFSET_X, VIDEO_Y - TOTEM_OFFSET_Y);
            }
            totem->set_zindex(-1.0f);
            totem_bottom->set_zindex(1.0f);
            //AddSprite(totem);
            //AddSprite(totem_bottom);
        }

    for(int i = 0; i < 3; ++i) {
        enemy_counter_[i] = new Sprite;
        enemy_counter_[i]->Initialize(number);
            
        enemy_counter_[i]->set_position(Vector2D(
                VIDEO_X/2 + (eye_image ? eye_image->width()/1.1 : 0) - NUMBER_WIDTH*(i+1),
                VIDEO_Y - (back_image ? back_image->height()/2 : 0) ));
        enemy_counter_[i]->set_zindex(1.0f);
        //AddSprite(enemy_counter_[i]);
        enemy_counter_value_[i] = 0;
    }

#ifdef DEBUG
    for(int i = 0; i < 3; ++i) {
        (fps_meter_[i] = new Sprite)->Initialize(number);
        fps_meter_[i]->set_position(Vector2D(
                        FPS_BAR_OFFSET_X + NUMBER_WIDTH*i + 0.0f, FPS_BAR_OFFSET_Y + 0.0f));
        //AddSprite(fps_meter_[i]);
        fps_meter_value_[i] = 0;
    }
#endif
    */
}

Hud::~Hud() {
    delete node_;
}

void Hud::Update(float delta_t) {
    World* world = WORLD();
    /*
    uint32 digit[7], enemy_number;
    enemy_number = (enemy_number = world->CountRemainingEnemies()) > 999 ? 999 : enemy_number;
    digit[2] = enemy_number / 100;
    digit[1] = (enemy_number / 10) % 10;
    digit[0] = enemy_number % 10;


    for(int i = 0; i < 3; ++i) {
        if(digit[i] != enemy_counter_value_[i]) {
            enemy_counter_value_[i] = digit[i];
            enemy_counter_[i]->SetDefaultFrame(enemy_counter_value_[i]);
        }
    }

#ifdef DEBUG
    int fps = Engine::reference()->current_fps();
    if(fps > 999) fps = 999;
    for(int i = 2; i >= 0; --i) {
        digit[i] = fps % 10;
        fps /= 10;
    }
    for(int i = 0; i < 3; ++i) {
        if(digit[i] != fps_meter_value_[i]) {
            fps_meter_value_[i] = digit[i];
            fps_meter_[i]->SetDefaultFrame(fps_meter_value_[i]);
        }
    }
#endif

    if (weapon_icon_ != NULL) {
        //icon_added[weapon_icon_]->set_visible(false);
    }

    if(world->hero() != NULL) {
        // Update the Selected weapon icon
        if(world->hero()->secondary_combat_art() != NULL)
            weapon_icon_ = world->hero()->secondary_combat_art()->icon();

        // Life Bar
        life_modifier_->set_offset(Vector2D(0.0f, -(((float) world->hero()->life()) / world->hero()->life().max_value()) * LIFE_BAR_HEIGHT) );

        // Mana Bar
        mana_modifier_->set_offset(Vector2D(0.0f, -(((float) world->hero()->mana()) / world->hero()->FullMana()) * MANA_BAR_HEIGHT) );

        block_modifier_->set_offset(Vector2D(0.0f, -(((float) world->hero()->mana_blocks().Get()) / world->hero()->mana_blocks().max_value()) * MANA_BAR_HEIGHT) );
    }

    if (weapon_icon_ != NULL && icon_added[weapon_icon_] == NULL) {
        /*Sprite* s = new Sprite;
        s->Initialize(weapon_icon_);

        s->set_hotspot(Vector2D(weapon_icon_->width()/2, weapon_icon_->height()/2));
        s->set_position(Vector2D(VIDEO_X/2 - 5, VIDEO_Y - 47));
        s->set_zindex(1.0f);
        
        //AddSprite(s);
        icon_added[weapon_icon_] = s;
    }
    //if(weapon_icon_)
    //    icon_added[weapon_icon_]->set_visible(true);
    */
}

}
