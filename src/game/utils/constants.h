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
    const static int   PROJECTILE_SPRITE_WIDTH = 32,
                       PROJECTILE_SPRITE_HEIGHT = 32,
                       PROJECTILE_DURATION = 5000,
                       
                       POTION_SPRITE_WIDTH = 20,
                       POTION_SPRITE_HEIGHT = 20,
                       POTION_RECOVER_LIFE = 1,
                       
                       HERO_MAX_LIFE = 5,
                       HERO_MAX_MANA = 5,
                       HERO_WIDTH = 110,
                       HERO_HEIGHT = 110,
                       HERO_HOTSPOT_X = 55,
                       HERO_HOTSPOT_Y = 102,

                       LIFE_IMAGE_WIDTH = 49,
                       LIFE_IMAGE_HEIGHT = 49,
                       LIFE_METER_OFFSET_X = -20,
                       LIFE_METER_OFFSET_Y = -20,
                       MANA_METER_OFFSET_X = 20,
                       MANA_METER_OFFSET_Y = -20,

                       DOOR_SPRITE_WIDTH = 153,
                       DOOR_SPRITE_HEIGHT = 109,

					   MUMMY_LIFE = 1,

                       BIG_MUMMY_LIFE = 3;

    const static float
        DOOR_HOTSPOT_WIDTH,
        DOOR_HOTSPOT_HEIGHT,
        DOOR_BOUND_WIDTH,
        DOOR_BOUND_HEIGHT,

        LIFE_BAR_WIDTH,
        LIFE_BAR_HEIGHT,
        MANA_BAR_WIDTH,
        MANA_BAR_HEIGHT,

        LIGHT_RADIUS_THRESHOLD,

        HERO_SPEED,

		PROJECTILE_SPEED,
		PROJECTILE_SPRITE_CENTER_X,
        PROJECTILE_SPRITE_CENTER_Y,
		PROJECTILE_HEIGHT,

        MUMMY_RADIUS,
        MUMMY_SPEED,

        BIG_MUMMY_RADIUS,
        BIG_MUMMY_SPEED,

		POTION_SPRITE_CENTER_X,
        POTION_SPRITE_CENTER_Y,
		POTION_HEIGHT;


  private:
    Constants() {}
};

}

#endif /* CONSTANTS_H_ */
