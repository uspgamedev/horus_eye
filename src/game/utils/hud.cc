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

using namespace framework;
using namespace scene;

#define EYE_WIDTH   50
#define EYE_HEIGHT  50

namespace utils {

Hud::Hud(World* world) {
    ImageFactory img_fac;
    Image* img = img_fac.LifeImage();
    img->set_frame_size(Vector2D(EYE_WIDTH, EYE_HEIGHT));
    animation_ = new Animation(10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                               13, 14, 15, 16, 17, 18, 19, 20, 21, 22, -1);

    life_icons_ = (Sprite**) malloc((world->hero()->max_life())*sizeof(*life_icons_));
    for(int i = 0; i < world->hero()->max_life(); ++i) {
        life_icons_[i] = new Sprite;
        life_icons_[i]->Initialize(img);

        life_icons_[i]->set_position(Vector2D(
                LIFE_METER_OFFSET_X + VIDEO_MANAGER()->video_size().x - LIFE_IMAGE_WIDTH*(i+1),
                LIFE_METER_OFFSET_Y + VIDEO_MANAGER()->video_size().y - LIFE_IMAGE_HEIGHT));
        life_icons_[i]->SelectAnimation(animation_);
        AddSprite(life_icons_[i]);
    }
}

Hud::~Hud() {
    World* world = WORLD();
    for(int i = 0; i < world->hero()->max_life(); ++i)
        free(life_icons_[i]);
    free(life_icons_);
}

void Hud::Update(float delta_t) {
    Layer::Update(delta_t);
    World* world = WORLD();
    if(world->hero() != NULL)
        for(int i = 0; i < world->hero()->max_life(); ++i)
            life_icons_[i]->set_visible(i < world->hero()->life());
}

}
