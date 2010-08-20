//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/hud.cc
// Implementacao da classe Hud.
//

#include "hud.h"
#include "../../framework/vector2D.h"
#include "../../framework/sprite.h"
#include "../scenes/world.h"
#include "../utils/imagefactory.h"
#include "constants.h"
#include "../../framework/videomanager.h"

#define LIFE_IMAGE_WIDTH Constants::LIFE_IMAGE_WIDTH
#define LIFE_IMAGE_HEIGHT Constants::LIFE_IMAGE_HEIGHT

#define LIFE_METER_OFFSET_X Constants::LIFE_METER_OFFSET_X
#define LIFE_METER_OFFSET_Y Constants::LIFE_METER_OFFSET_Y
#define LIFE_BAR_WIDTH Constants::LIFE_BAR_WIDTH
#define LIFE_BAR_HEIGHT Constants::LIFE_BAR_HEIGHT

#define MANA_METER_OFFSET_X Constants::MANA_METER_OFFSET_X
#define MANA_METER_OFFSET_Y Constants::MANA_METER_OFFSET_Y
#define MANA_BAR_WIDTH Constants::MANA_BAR_WIDTH
#define MANA_BAR_HEIGHT Constants::MANA_BAR_HEIGHT

#define ENEMY_COUNTER_OFFSET_X -10
#define ENEMY_COUNTER_OFFSET_Y  30

#define FPS_METER_OFFSET_X  10
#define FPS_METER_OFFSET_Y  30

using namespace framework;
using namespace scene;

#define EYE_WIDTH   50
#define EYE_HEIGHT  50
#define NUMBER_WIDTH 18
#define NUMBER_HEIGHT 16

namespace utils {

// Aviso: maximo de 99 mumias no display de inimigos restantes!
Hud::Hud(World* world) {
    Image* number = VIDEO_MANAGER()->LoadImage("data/images/numbers2.png");
    Image* slash = VIDEO_MANAGER()->LoadImage("data/images/slash2.png");
    number->set_frame_size(Vector2D(NUMBER_WIDTH, NUMBER_HEIGHT));
    /*ImageFactory img_fac;
    Image* img = img_fac.LifeImage();
    img->set_frame_size(Vector2D(EYE_WIDTH, EYE_HEIGHT));
    animation_ = new Animation(10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                               13, 14, 15, 16, 17, 18, 19, 20, 21, 22, -1);

    life_icons_ = (Sprite**) malloc((world->hero()->max_life())*sizeof(*life_icons_));
    icon_count_ = world->hero()->max_life();
    for(int i = 0; i < icon_count_; ++i) {
        life_icons_[i] = new Sprite;
        life_icons_[i]->Initialize(img);

        life_icons_[i]->set_position(Vector2D(
                LIFE_METER_OFFSET_X + VIDEO_MANAGER()->video_size().x - LIFE_IMAGE_WIDTH*(i+1),
                LIFE_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - LIFE_IMAGE_HEIGHT));
        life_icons_[i]->SelectAnimation(animation_);
        life_icons_[i]->set_visible(i < world->hero()->life());
        //AddSprite(life_icons_[i]);
    }*/

    //Criando sprites da life bar
    life_bar_[0] = new Sprite;
    life_bar_images_[0] = new Image;
    float bar_width = ((float)world->hero()->life() * LIFE_BAR_WIDTH) / ((float)world->hero()->max_life());
    life_bar_images_[0]->Create(Vector2D(bar_width, LIFE_BAR_HEIGHT));
    life_bar_images_[0]->Clear(0x00FF0000);
    life_bar_[0]->Initialize(life_bar_images_[0]);
    life_bar_[0]->set_position(Vector2D(
            LIFE_METER_OFFSET_X + VIDEO_MANAGER()->video_size().x - bar_width,
            LIFE_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - LIFE_BAR_HEIGHT));

    life_bar_[1] = new Sprite;
    //TODO: alterar esta image (life_bar_images_[1]) pra ser a imagem do "recipiente" da barra.
    //      lembrar que ao ser pega de um arquivo, nao precisamos deleta-la no destrutor.
    life_bar_images_[1] = new Image;
    life_bar_images_[1]->Create(Vector2D(LIFE_BAR_WIDTH, LIFE_BAR_HEIGHT));
    life_bar_images_[1]->Clear(0x00000000);
    life_bar_[1]->Initialize(life_bar_images_[1]);
    life_bar_[1]->set_position(Vector2D(
            LIFE_METER_OFFSET_X + VIDEO_MANAGER()->video_size().x - LIFE_BAR_WIDTH,
            LIFE_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - LIFE_BAR_HEIGHT));

    //Criando sprites da mana bar
    mana_bar_[0] = new Sprite;
    mana_bar_images_[0] = new Image;
    bar_width = ((float)world->hero()->mana() * MANA_BAR_WIDTH) / ((float)world->hero()->max_mana());
    mana_bar_images_[0]->Create(Vector2D(bar_width, MANA_BAR_HEIGHT));
    mana_bar_images_[0]->Clear(0x000000FF);
    mana_bar_[0]->Initialize(mana_bar_images_[0]);
    mana_bar_[0]->set_position(Vector2D(
            MANA_METER_OFFSET_X,
            MANA_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - MANA_BAR_HEIGHT));

    mana_bar_[1] = new Sprite;
    //TODO: alterar esta image (life_bar_images_[1]) pra ser a imagem do "recipiente" da barra.
    //      lembrar que ao ser pega de um arquivo, nao precisamos deleta-la no destrutor.
    mana_bar_images_[1] = new Image;
    mana_bar_images_[1]->Create(Vector2D(MANA_BAR_WIDTH, MANA_BAR_HEIGHT));
    mana_bar_images_[1]->Clear(0x00000000);
    mana_bar_[1]->Initialize(mana_bar_images_[1]);
    mana_bar_[1]->set_position(Vector2D(
            MANA_METER_OFFSET_X,
            MANA_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - MANA_BAR_HEIGHT));

    AddSprite(life_bar_[1]);
    AddSprite(life_bar_[0]);
    AddSprite(mana_bar_[1]);
    AddSprite(mana_bar_[0]);


    for(int i = 0; i < 5; ++i) {
        enemy_counter_[i] = new Sprite;
        if(i != 2) { // 2 == Slash
            enemy_counter_[i]->Initialize(number);
            enemy_animation_[i] = new Animation(50, 0, -1);
            enemy_counter_[i]->SelectAnimation(enemy_animation_[i]);
        } else {
            enemy_counter_[i]->Initialize(slash);
            enemy_animation_[i] = NULL;
        }
        enemy_counter_[i]->set_position(Vector2D(
                ENEMY_COUNTER_OFFSET_X + VIDEO_MANAGER()->video_size().x - NUMBER_WIDTH*(i+1),
                ENEMY_COUNTER_OFFSET_Y ));
        AddSprite(enemy_counter_[i]);
        enemy_counter_value_[i] = 0;
    }

    for(int i = 0; i < 4; ++i) {
        (fps_meter_[i] = new Sprite)->Initialize(number);
        fps_meter_[i]->SelectAnimation(fps_animation_[i] = new Animation(50, 0, -1));
        fps_meter_[i]->set_position(Vector2D(
                        FPS_METER_OFFSET_X + NUMBER_WIDTH*i, FPS_METER_OFFSET_Y ));
        AddSprite(fps_meter_[i]);
        fps_meter_value_[i] = 0;
    }
}

Hud::~Hud() {
    //for(int i = 0; i < icon_count_; ++i)
    //    free(life_icons_[i]);
    //free(life_icons_);
    //delete animation_;
    life_bar_images_[0]->Destroy();
    delete life_bar_images_[0];
    life_bar_images_[1]->Destroy();
    delete life_bar_images_[1];

    mana_bar_images_[0]->Destroy();
    delete mana_bar_images_[0];
    mana_bar_images_[1]->Destroy();
    delete mana_bar_images_[1];

    sprite_list_.clear();
}

void Hud::Update(float delta_t) {
    Layer::Update(delta_t);
    World* world = WORLD();

    int newval[5];
    int temp = world->CountRemainingEnemies();
    temp = temp > 99 ? 99 : temp;
    newval[4] = temp / 10;
    newval[3] = temp % 10;
    newval[2] = 0;
    temp = (temp = world->max_enemies()) > 99 ? 99 : temp;
    newval[1] = temp / 10;
    newval[0] = temp % 10;

    for(int i = 0; i < 5; ++i)
        if(newval[i] != enemy_counter_value_[i]) {
            enemy_counter_value_[i] = newval[i];
            delete enemy_animation_[i];
            enemy_animation_[i] = new Animation(50, enemy_counter_value_[i], -1);
            enemy_counter_[i]->SelectAnimation(enemy_animation_[i]);
        }

    int fps = Engine::reference()->current_fps();
    if(fps > 9999) fps = 9999;
    for(int i = 3; i >= 0; --i) {
        newval[i] = fps % 10;
        fps /= 10;
    }
    for(int i = 0; i < 4; ++i) {
        if(newval[i] != fps_meter_value_[i]) {
            fps_meter_value_[i] = newval[i];
            delete fps_animation_[i];
            fps_animation_[i] = new Animation(50, fps_meter_value_[i], -1);
            fps_meter_[i]->SelectAnimation(fps_animation_[i]);
        }
    }

    if(world->hero() != NULL && world->hero()->life() > 0)
    {
        //for(int i = 0; i < world->hero()->max_life(); ++i)
        float new_width = ((float)world->hero()->life() * LIFE_BAR_WIDTH) / ((float)world->hero()->max_life());
        life_bar_[0]->image()->set_frame_size(Vector2D(
                new_width,
                LIFE_BAR_HEIGHT
                ));
        life_bar_[0]->set_position(Vector2D(
                LIFE_METER_OFFSET_X + VIDEO_MANAGER()->video_size().x - new_width,
                LIFE_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - LIFE_BAR_HEIGHT));

        new_width = ((float)world->hero()->mana() * MANA_BAR_WIDTH) / ((float)world->hero()->max_mana());
        mana_bar_[0]->image()->set_frame_size(Vector2D(
                new_width,
                MANA_BAR_HEIGHT
                ));
            //life_icons_[i]->set_visible(i < world->hero()->life());
    }
}

}
