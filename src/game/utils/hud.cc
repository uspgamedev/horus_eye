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
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../scenes/world.h"
#include "../utils/imagefactory.h"
#include "constants.h"

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

#define NUMBER_WIDTH 18
#define NUMBER_HEIGHT 16

using namespace framework;
using namespace scene;

namespace utils {

// Aviso: maximo de 99 mumias no display de inimigos restantes!
Hud::Hud(World* world) {
    Image* number = VIDEO_MANAGER()->LoadImage("data/images/numbers2.png");
    Image* slash = VIDEO_MANAGER()->LoadImage("data/images/slash2.png");
    number->set_frame_size(Vector2D(NUMBER_WIDTH, NUMBER_HEIGHT));

    //Criando sprites da life bar
    life_bar_[0] = new Sprite;
    life_bar_images_[0] = new Image;
    float bar_width = ((float)world->hero()->life() * LIFE_BAR_WIDTH) / ((float)world->hero()->max_life());
    life_bar_images_[0]->Create(Vector2D(LIFE_BAR_WIDTH, LIFE_BAR_HEIGHT));
    life_bar_images_[0]->Clear(0x00FF0000);
    life_bar_[0]->Initialize(life_bar_images_[0]);
    life_bar_[0]->set_position(Vector2D(
            LIFE_METER_OFFSET_X + VIDEO_MANAGER()->video_size().x - bar_width,
            LIFE_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - LIFE_BAR_HEIGHT));
	life_bar_[0]->image()->set_frame_size(Vector2D(bar_width, LIFE_BAR_HEIGHT));
	

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
    mana_bar_images_[0]->Create(Vector2D(MANA_BAR_WIDTH, MANA_BAR_HEIGHT));
    mana_bar_images_[0]->Clear(0x000000FF);
    mana_bar_[0]->Initialize(mana_bar_images_[0]);
    mana_bar_[0]->set_position(Vector2D(
            MANA_METER_OFFSET_X + 0.0f,
            MANA_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - MANA_BAR_HEIGHT));
	mana_bar_[0]->image()->set_frame_size(Vector2D(bar_width, MANA_BAR_HEIGHT));
	mana_bar_[0]->set_visible(false);

    mana_bar_[1] = new Sprite;
    //TODO: alterar esta image (life_bar_images_[1]) pra ser a imagem do "recipiente" da barra.
    //      lembrar que ao ser pega de um arquivo, nao precisamos deleta-la no destrutor.
    mana_bar_images_[1] = new Image;
    mana_bar_images_[1]->Create(Vector2D(MANA_BAR_WIDTH, MANA_BAR_HEIGHT));
    mana_bar_images_[1]->Clear(0x00000000);
    mana_bar_[1]->Initialize(mana_bar_images_[1]);
    mana_bar_[1]->set_position(Vector2D(
            MANA_METER_OFFSET_X + 0.0f,
            MANA_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - MANA_BAR_HEIGHT));

    AddSprite(life_bar_[1]);
    AddSprite(life_bar_[0]);
    AddSprite(mana_bar_[1]);
    AddSprite(mana_bar_[0]);


    for(int i = 0; i < 7; ++i) {
        enemy_counter_[i] = new Sprite;
        if(i != 3) // 3 == Slash
            enemy_counter_[i]->Initialize(number);
        else
            enemy_counter_[i]->Initialize(slash);
        enemy_counter_[i]->set_position(Vector2D(
                ENEMY_COUNTER_OFFSET_X + VIDEO_MANAGER()->video_size().x - NUMBER_WIDTH*(i+1),
                ENEMY_COUNTER_OFFSET_Y ));
        AddSprite(enemy_counter_[i]);
        enemy_counter_value_[i] = 0;
    }

    for(int i = 0; i < 3; ++i) {
        (fps_meter_[i] = new Sprite)->Initialize(number);
        fps_meter_[i]->set_position(Vector2D(
                        FPS_METER_OFFSET_X + NUMBER_WIDTH*i + 0.0f, FPS_METER_OFFSET_Y + 0.0f));
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

    int newval[7], temp;
    temp = (temp = world->CountRemainingEnemies()) > 999 ? 999 : temp;
    newval[6] = temp / 100;
    newval[5] = (temp / 10) % 10;
    newval[4] = temp % 10;
    newval[3] = 0;
    temp = (temp = world->max_enemies()) > 999 ? 999 : temp;
    newval[2] = temp / 100;
    newval[1] = (temp / 10) % 10;
    newval[0] = temp % 10;

    for(int i = 0; i < 7; ++i)
        if(newval[i] != enemy_counter_value_[i]) {
            enemy_counter_value_[i] = newval[i];
            enemy_counter_[i]->animation()->set_framelist(enemy_counter_value_[i], -1);
        }

    int fps = Engine::reference()->current_fps();
    if(fps > 999) fps = 999;
    for(int i = 2; i >= 0; --i) {
        newval[i] = fps % 10;
        fps /= 10;
    }
    for(int i = 0; i < 3; ++i) {
        if(newval[i] != fps_meter_value_[i]) {
            fps_meter_value_[i] = newval[i];
            fps_meter_[i]->animation()->set_framelist(fps_meter_value_[i], -1);
        }
    }

    if(world->hero() != NULL) {
        if(world->hero()->life() > 0) {
            float new_width = ((float)world->hero()->life() * LIFE_BAR_WIDTH) / ((float)world->hero()->max_life());

            life_bar_[0]->image()->set_frame_size(Vector2D(
                    new_width,
                    LIFE_BAR_HEIGHT
                    ));
            life_bar_[0]->set_position(Vector2D(
                    LIFE_METER_OFFSET_X + VIDEO_MANAGER()->video_size().x - new_width,
                    LIFE_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - LIFE_BAR_HEIGHT));
        }
        if(world->hero()->mana() > 0) {
            mana_bar_[0]->set_visible(true);
            float new_width = ((float)world->hero()->mana() * MANA_BAR_WIDTH) / ((float)world->hero()->max_mana());
            mana_bar_[0]->image()->set_frame_size(Vector2D(
                    new_width,
                    MANA_BAR_HEIGHT
                    ));
        } else //if(world->hero()->mana() == 0)
            mana_bar_[0]->set_visible(false);
    }
}

}
