#include "constants.h"

namespace utils {

const float Constants::DOOR_HOTSPOT_WIDTH = 153.0f/2.0f;
const float Constants::DOOR_HOTSPOT_HEIGHT = 109.0f-(91.0f/2.0f);
const float Constants::DOOR_BOUND_WIDTH = 2.0f;
const float Constants::DOOR_BOUND_HEIGHT = 1.0f;

const float Constants::CONDITION_DURATION = 30.0f;

const float Constants::FLOOR_WIDTH = 106.0f;
const float Constants::FLOOR_HEIGHT =  54.0f;
const float Constants::FLOOR_HOTSPOT_X = 106.0f/2.0f;
const float Constants::FLOOR_HOTSPOT_Y =  54.0f/2.0f;

const float Constants::WALL_HOTSPOT_X = 106.0f/2.0f;
const float Constants::WALL_HOTSPOT_Y = 157.0f - 27.0f;

const float Constants::LIFE_BAR_WIDTH = 22.0f;
const float Constants::LIFE_BAR_HEIGHT = 456.0f;
const float Constants::MANA_BAR_WIDTH = 22.0f;
const float Constants::MANA_BAR_HEIGHT = 465.0f;
const float Constants::TOTEM_WIDTH = 72.0f;
const float Constants::TOTEM_HEIGHT = 518.0f;

const float Constants::LIGHT_RADIUS_THRESHOLD = 0.25f;
const float Constants::LIGHT_RADIUS_INITIAL = 6.0f;

const float Constants::HERO_SPEED = 4.0f;

const float Constants::PROJECTILE_SPEED = 8.0f;
const float Constants::PROJECTILE_SPRITE_CENTER_X = 16.0f;
const float Constants::PROJECTILE_SPRITE_CENTER_Y = 16.0f;
const float Constants::PROJECTILE_HEIGHT = 12.0f;

const float Constants::EXPLOSION_SPEED = 8.0f;
const float Constants::EXPLOSION_SPRITE_CENTER_X = 128.0f;
const float Constants::EXPLOSION_SPRITE_CENTER_Y = 64.0f;
const float Constants::EXPLOSION_WIDTH = 256.0f;
const float Constants::EXPLOSION_HEIGHT = 128.0f;

const float Constants::FIREBALL_EXPLOSION_RADIUS = 1.6f;
const float Constants::QUAKE_EXPLOSION_RADIUS = 1.25f;
const float Constants::QUAKE_EXPLOSION_RANGE = 6.0f;

const float Constants::SIGHT_POTION_INCREASE = 1.2f;

const float Constants::MUMMY_SIGHT_RANGE = 20.0f;

const float Constants::MUMMY_RADIUS = 0.3f;
const float Constants::MUMMY_SPEED = 2.9f;

const float Constants::RANGED_MUMMY_RANGE = 5.0f;

const float Constants::BIG_MUMMY_RADIUS = 0.5f;
const float Constants::BIG_MUMMY_SPEED = 2.0f;

const float Constants::PHARAOH_RADIUS = 0.45f;
const float Constants::PHARAOH_SPEED = 3.3f;
const float Constants::PHARAOH_SUMMON_RANGE = 2.0f;
const float Constants::PHARAOH_MANA_REGEN_RATE = 2.0f;

const float Constants::POTION_SPRITE_CENTER_X = 15.0f;
const float Constants::POTION_SPRITE_CENTER_Y = 15.0f;
const float Constants::POTION_HEIGHT = 16.0f;

const std::string Constants::CONFIGURATION_FILE = "settings.bin";
const std::string Constants::BACKGROUND_MUSIC = "data/musics/bgmusic.wav";
const std::string Constants::VERSION = "v1.0.1";
}
