//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/constants.h
// Definicao da classe Constants.
//

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace utils {

class Constants {
  public:

    const static int   PROJECTILE_SPRITE_CENTER_X = 16,
                       PROJECTILE_SPRITE_CENTER_Y = 16,
                       PROJECTILE_SPRITE_WIDTH = 32,
                       PROJECTILE_SPRITE_HEIGHT = 32,
                       PROJECTILE_HEIGHT = 12,
                       PROJECTILE_DURATION = 5000,

                       HERO_MAX_LIFE = 5,
                       HERO_WIDTH = 110,
                       HERO_HEIGHT = 110,
                       HERO_HOTSPOT_X = 55,
                       HERO_HOTSPOT_Y = 102,

                       LIFE_IMAGE_WIDTH = 49,
                       LIFE_IMAGE_HEIGHT = 49,
                       LIFE_METER_OFFSET_X = -20,
                       LIFE_METER_OFFSET_Y = -20,

                       DOOR_SPRITE_WIDTH = 153,
                       DOOR_SPRITE_HEIGHT = 109,

                       BIG_MUMMY_LIFE = 3;

    const static float DOOR_HOTSPOT_WIDTH = 153/2.0,
                       DOOR_HOTSPOT_HEIGHT = 109-(91/2.0),
                       DOOR_BOUND_WIDTH = 2.0,
                       DOOR_BOUND_HEIGHT = 1.0,

                       HERO_SPEED = 3.0f,
                       PROJECTILE_SPEED = 8.0f,

                       BIG_MUMMY_RADIUS = 0.5f,
                       BIG_MUMMY_SPEED = 1.7f;

  private:
    Constants() {}
};

}

#endif /* CONSTANTS_H_ */
