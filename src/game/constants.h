#ifndef HORUSEYE_GAME_CONSTANTS_H_
#define HORUSEYE_GAME_CONSTANTS_H_

#include <string>

namespace constants {

void RegisterInt(const std::string& key, int value);
void RegisterDouble(const std::string& key, double value);
void RegisterString(const std::string& key, const std::string& value);

int GetInt(const std::string& key);
double GetDouble(const std::string& key);
std::string GetStr(const std::string& key);


std::string data_location ();
std::string binary_configuration_filename ();
std::string ini_configuration_filename ();
std::string background_music ();
std::string version ();


/*class Constants {
  public:
    const static int
        PROJECTILE_SPRITE_WIDTH = 32,
        PROJECTILE_SPRITE_HEIGHT = 32,
        PROJECTILE_DURATION = 4,

        EXPLOSION_SPRITE_WIDTH = 128,
        EXPLOSION_SPRITE_HEIGHT = 256,
        EXPLOSION_DURATION = 3000,

        LIGHTNING_SPRITE_WIDTH = 64,
        LIGHTNING_SPRITE_HEIGHT = 64,
        LIGHTNING_DURATION = 3500,

        FIREBALL_SPRITE_WIDTH = 64,
        FIREBALL_SPRITE_HEIGHT = 64,
        FIREBALL_DURATION = 3500,
                       
        PROJECTILE_DAMAGE = 200,

        LIFEPOTION_RECOVER_LIFE = 200,
        MANAPOTION_RECOVER_MANA = 200,
                       
        HERO_WIDTH = 110,
        HERO_HEIGHT = 110,
        HERO_MAX_WEAPONS = 6,
        HERO_MIN_MANA_BLOCKS = 1,
        HERO_MAX_MANA_BLOCKS = 5,
        HERO_BASE_MANA_REGEN_RATIO = 3,

        SIGHT_POTION_MAX_STACK = 5,

        LIFE_BAR_OFFSET_X = 36,
        LIFE_BAR_OFFSET_Y = 100,
        MANA_BAR_OFFSET_X = 36,
        MANA_BAR_OFFSET_Y = 100,                 
        TOTEM_OFFSET_X = 36,
        TOTEM_OFFSET_Y = 80,

        MUMMY_LIFE = 400,
        MUMMY_DAMAGE = 200,

        RANGED_MUMMY_LIFE = 400,
        RANGED_MUMMY_DAMAGE = 200,

        BIG_MUMMY_LIFE = 1200,
        BIG_MUMMY_DAMAGE = 300,

        PAPER_MUMMY_LIFE = 1,
        PAPER_MUMMY_DAMAGE = 25,

        FIREBALL_EXPLOSION_DAMAGE = 200,
        FIREBALL_COST = 200,
        QUAKE_EXPLOSION_DAMAGE = 400,
        QUAKE_COST = 100,
        QUAKE_BLOCK_COST = 1,
        LIGHTNING_DAMAGE = 150,
        LIGHTNING_COST = 100,
        METEOR_EXPLOSION_DAMAGE = 500,
        METEOR_COST = 200,
        METEOR_BLOCK_COST = 1,

        PHARAOH_LIFE = 6000,
        PHARAOH_MANA = 200,
        PHARAOH_SUMMON_MANA_COST = 60,
        PHARAOH_DAMAGE = 400,
        PHARAOH_RANGED_DAMAGE = 200;


    const static double
        DOOR_HOTSPOT_WIDTH,
        DOOR_HOTSPOT_HEIGHT,
        DOOR_BOUND_WIDTH,
        DOOR_BOUND_HEIGHT,
        
        CONDITION_DURATION,

        FLOOR_WIDTH,
        FLOOR_HEIGHT,
        FLOOR_HOTSPOT_X,
        FLOOR_HOTSPOT_Y,

        WALL_HOTSPOT_X,
        WALL_HOTSPOT_Y,

        LIFE_BAR_WIDTH,
        LIFE_BAR_HEIGHT,
        MANA_BAR_WIDTH,
        MANA_BAR_HEIGHT,
        TOTEM_WIDTH,
        TOTEM_HEIGHT,
        
        FIREBALL_EXPLOSION_RADIUS,
        QUAKE_EXPLOSION_RADIUS,
        QUAKE_EXPLOSION_RANGE,
        METEOR_EXPLOSION_RADIUS,

        LIGHT_RADIUS_THRESHOLD,
        LIGHT_RADIUS_INITIAL,

        HERO_SPEED,
        HERO_MAX_LIFE,
        HERO_MANA_PER_BLOCK,
        HERO_MANA_REGEN_BASE,

        PROJECTILE_SPEED,
        PROJECTILE_SPRITE_CENTER_X,
        PROJECTILE_SPRITE_CENTER_Y,
        PROJECTILE_HEIGHT,

        LIGHTNING_SPEED,
        LIGHTNING_SPRITE_CENTER_X,
        LIGHTNING_SPRITE_CENTER_Y,
        LIGHTNING_HEIGHT,

        FIREBALL_SPEED,
        FIREBALL_SPRITE_CENTER_X,
        FIREBALL_SPRITE_CENTER_Y,
        FIREBALL_HEIGHT,
              
        EXPLOSION_SPEED,
        EXPLOSION_SPRITE_CENTER_X,
        EXPLOSION_SPRITE_CENTER_Y,
        EXPLOSION_WIDTH,
        EXPLOSION_HEIGHT,

        SANDSTORM_FADEOUT_TIME,
        SANDSTORM_PROJECTILE_INTERVAL,
        SANDSTORM_COST,
        SANDSTORM_MAINTAIN_COST,
        SANDSTORM_ANGLE,
        SANDSTORM_LENGHT,

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
        PHARAOH_MANA_REGEN,

        POTION_SPRITE_CENTER_X,
        POTION_SPRITE_CENTER_Y,
        POTION_HEIGHT;

    const static std::string
        DATA_LOCATION,
        BINARY_CONFIGURATION_FILENAME,
        INI_CONFIGURATION_FILENAME,
        BACKGROUND_MUSIC;

    const static std::wstring
        VERSION;


  private:
    Constants() {}
};*/

}

#endif /* HORUSEYE_GAME_CONSTANTS_H_ */
