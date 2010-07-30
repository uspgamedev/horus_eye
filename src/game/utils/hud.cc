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

#define ENEMY_COUNTER_OFFSET_X -10
#define ENEMY_COUNTER_OFFSET_Y  30

using namespace framework;
using namespace scene;

#define EYE_WIDTH   50
#define EYE_HEIGHT  50
#define NUMBER_WIDTH 21
#define NUMBER_HEIGHT 16

namespace utils {

// Aviso: maximo de 99 mumias no display de inimigos restantes!
Hud::Hud(World* world) : icon_count_(0) {
    Image* number = VIDEO_MANAGER()->LoadImage("data/images/numbers.png");
    Image* slash = VIDEO_MANAGER()->LoadImage("data/images/slash.png");
    number->set_frame_size(Vector2D(NUMBER_WIDTH, NUMBER_HEIGHT));
    ImageFactory img_fac;
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
        AddSprite(life_icons_[i]);
    }

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
}

Hud::~Hud() {
    for(int i = 0; i < icon_count_; ++i)
        free(life_icons_[i]);
    free(life_icons_);
    delete animation_;
    sprite_list_.clear();
}

void Hud::Update(float delta_t) {
    Layer::Update(delta_t);
    World* world = WORLD();

    int newval[5];
    newval[4] = world->CountRemainingEnemies() / 10;
    newval[3] = world->CountRemainingEnemies() % 10;
    newval[2] = 0;
    newval[1] = world->max_enemies() / 10;
    newval[0] = world->max_enemies() % 10;

    for(int i = 0; i < 5; ++i)
        if(newval[i] != enemy_counter_value_[i]) {
            enemy_counter_value_[i] = newval[i];
            delete enemy_animation_[i];
            enemy_animation_[i] = new Animation(50, enemy_counter_value_[i], -1);
        }
    if(world->hero() != NULL)
        for(int i = 0; i < world->hero()->max_life(); ++i)
            life_icons_[i]->set_visible(i < world->hero()->life());
}

}
