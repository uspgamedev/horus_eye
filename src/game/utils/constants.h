//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/constants.h
// Definicao da classe Constants.
//

#ifndef HORUSEYE_GAME_UTILS_CONSTANTS_H_
#define HORUSEYE_GAME_UTILS_CONSTANTS_H_

namespace utils {

class Constants {
  public:
    const static int   PROJECTILE_SPRITE_WIDTH = 32,
                       PROJECTILE_SPRITE_HEIGHT = 32,
                       PROJECTILE_DURATION = 3500,

                       EXPLOSION_SPRITE_WIDTH = 128,
                       EXPLOSION_SPRITE_HEIGHT = 256,
                       EXPLOSION_DURATION = 3000,
                       
                       PROJECTILE_DAMAGE = 10,
                       EXPLOSION_DAMAGE = 1,

                       LIFEPOTION_RECOVER_LIFE = 10,
                       MANAPOTION_RECOVER_MANA = 10,
                       
                       HERO_MAX_LIFE = 50,
                       HERO_MAX_MANA = 50,
                       HERO_WIDTH = 110,
                       HERO_HEIGHT = 110,
                       HERO_HOTSPOT_X = 55,
                       HERO_HOTSPOT_Y = 102,
                       HERO_MAX_WEAPONS = 5,
                       SIGHT_POTION_MAX_STACK = 5,

                       LIFE_IMAGE_WIDTH = 49,
                       LIFE_IMAGE_HEIGHT = 49,
                       LIFE_METER_OFFSET_X = -20,
                       LIFE_METER_OFFSET_Y = -20,
                       MANA_METER_OFFSET_X = 20,
                       MANA_METER_OFFSET_Y = -20,                 

					   MUMMY_LIFE = 10,
					   MUMMY_DAMAGE = 10,

                       RANGED_MUMMY_LIFE = 10,
                       RANGED_MUMMY_DAMAGE = 10,

                       BIG_MUMMY_LIFE = 30,
                       BIG_MUMMY_DAMAGE = 15,

                       FIREBALL_EXPLOSION_DAMAGE = 15,
                       FIREBALL_COST = 10,
                       QUAKE_EXPLOSION_DAMAGE = 20,
                       QUAKE_COST = 15,

					   PHARAOH_LIFE = 175,
					   PHARAOH_MANA = 10,
					   PHARAOH_SUMMON_MANA_COST = 3,
					   PHARAOH_DAMAGE = 20,
					   PHARAOH_RANGED_DAMAGE = 10;


    const static float
        DOOR_HOTSPOT_WIDTH,
        DOOR_HOTSPOT_HEIGHT,
        DOOR_BOUND_WIDTH,
        DOOR_BOUND_HEIGHT,
        
        CONDITION_DURATION,

        FLOOR_HOTSPOT_X,
        FLOOR_HOTSPOT_Y,

        WALL_HOTSPOT_X,
        WALL_HOTSPOT_Y,

        LIFE_BAR_WIDTH,
        LIFE_BAR_HEIGHT,
        MANA_BAR_WIDTH,
        MANA_BAR_HEIGHT,
        
        FIREBALL_EXPLOSION_RADIUS,
        QUAKE_EXPLOSION_RADIUS,

        LIGHT_RADIUS_THRESHOLD,
        LIGHT_RADIUS_INITIAL,

        HERO_SPEED,

		PROJECTILE_SPEED,
		PROJECTILE_SPRITE_CENTER_X,
        PROJECTILE_SPRITE_CENTER_Y,
		PROJECTILE_HEIGHT,

		EXPLOSION_SPEED,
		EXPLOSION_SPRITE_CENTER_X,
        EXPLOSION_SPRITE_CENTER_Y,
		EXPLOSION_WIDTH,
		EXPLOSION_HEIGHT,

		SIGHT_POTION_INCREASE,

		MUMMY_SIGHT_RANGE,

        MUMMY_RADIUS,
        MUMMY_SPEED,

        RANGED_MUMMY_RANGE,

        BIG_MUMMY_RADIUS,
        BIG_MUMMY_SPEED,

		PHARAOH_RADIUS,
		PHARAOH_SPEED,
		PHARAOH_SUMMON_RANGE,
		PHARAOH_MANA_REGEN_RATE,

		POTION_SPRITE_CENTER_X,
        POTION_SPRITE_CENTER_Y,
		POTION_HEIGHT;


  private:
    Constants() {}
};

}

#endif /* HORUSEYE_GAME_UTILS_CONSTANTS_H_ */
