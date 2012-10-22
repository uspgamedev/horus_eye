#include <ugdk/portable/tr1.h>
#include FROM_TR1(unordered_map)
#include "constants.h"
#include "game/config.h"

namespace utils {

using std::string;
using std::unordered_map;

static unordered_map<string, int> int_db_;
static unordered_map<string, double> double_db_;
static unordered_map<string, string> str_db_;

void RegisterInt(const string& key, int value) {
    int_db_[key] = value;
}
void RegisterDouble(const string& key, double value) {
    double_db_[key] = value;
}
void RegisterString(const string& key, const std::string& value) {
    str_db_[key] = value;
}

int GetInt(const string& key) {
}
double GetDouble(const string& key) {
}
string GetStr(const string& key) {
}

/*const double Constants::DOOR_HOTSPOT_WIDTH = 153.0/2.0;
const double Constants::DOOR_HOTSPOT_HEIGHT = 109.0-(91.0/2.0);
const double Constants::DOOR_BOUND_WIDTH = 2.0;
const double Constants::DOOR_BOUND_HEIGHT = 1.0;

const double Constants::CONDITION_DURATION = 30.0;

const double Constants::FLOOR_WIDTH = 106.0;
const double Constants::FLOOR_HEIGHT =  54.0;
const double Constants::FLOOR_HOTSPOT_X = 106.0/2.0;
const double Constants::FLOOR_HOTSPOT_Y =  54.0/2.0;

const double Constants::WALL_HOTSPOT_X = 106.0/2.0;
const double Constants::WALL_HOTSPOT_Y = 157.0 - 27.0;

const double Constants::LIFE_BAR_WIDTH = 22.0;
const double Constants::LIFE_BAR_HEIGHT = 456.0;
const double Constants::MANA_BAR_WIDTH = 22.0;
const double Constants::MANA_BAR_HEIGHT = 465.0;
const double Constants::TOTEM_WIDTH = 72.0;
const double Constants::TOTEM_HEIGHT = 518.0;

const double Constants::LIGHT_RADIUS_THRESHOLD = 0.25;
const double Constants::LIGHT_RADIUS_INITIAL = 6.0;

const double Constants::HERO_SPEED = 4.0;
const double Constants::HERO_MAX_LIFE = 1000.0;
const double Constants::HERO_MANA_PER_BLOCK = 200.0;
const double Constants::HERO_MANA_REGEN_BASE = 20.0;

const double Constants::PROJECTILE_SPEED = 8.0;
const double Constants::PROJECTILE_SPRITE_CENTER_X = 16.0;
const double Constants::PROJECTILE_SPRITE_CENTER_Y = 16.0;
const double Constants::PROJECTILE_HEIGHT = 12.0;

const double Constants::LIGHTNING_SPEED = 24.0;
const double Constants::LIGHTNING_SPRITE_CENTER_X = 32.0;
const double Constants::LIGHTNING_SPRITE_CENTER_Y = 32.0;
const double Constants::LIGHTNING_HEIGHT = 12.0;

const double Constants::FIREBALL_SPEED = 10.0;
const double Constants::FIREBALL_SPRITE_CENTER_X = 32.0;
const double Constants::FIREBALL_SPRITE_CENTER_Y = 32.0;
const double Constants::FIREBALL_HEIGHT = 12.0;

const double Constants::EXPLOSION_SPEED = 8.0;
const double Constants::EXPLOSION_SPRITE_CENTER_X = 128.0;
const double Constants::EXPLOSION_SPRITE_CENTER_Y = 64.0;
const double Constants::EXPLOSION_WIDTH = 256.0;
const double Constants::EXPLOSION_HEIGHT = 128.0;

const double Constants::FIREBALL_EXPLOSION_RADIUS = 1.6;
const double Constants::QUAKE_EXPLOSION_RADIUS = 1.25;
const double Constants::QUAKE_EXPLOSION_RANGE = 6.0;
const double Constants::METEOR_EXPLOSION_RADIUS = 2.0;

const double Constants::SANDSTORM_FADEOUT_TIME = 0.50;
const double Constants::SANDSTORM_PROJECTILE_INTERVAL = 0.125;
const double Constants::SANDSTORM_COST = 200.0;
const double Constants::SANDSTORM_MAINTAIN_COST = 75.0;
const double Constants::SANDSTORM_ANGLE = PI/6.0;
const double Constants::SANDSTORM_LENGHT = 2.5;

const double Constants::SIGHT_POTION_INCREASE = 1.2;

const double Constants::MUMMY_SIGHT_RANGE = 20.0;

const double Constants::MUMMY_RADIUS = 0.3;
const double Constants::MUMMY_SPEED = 2.9;

const double Constants::RANGED_MUMMY_RANGE = 5.0;

const double Constants::BIG_MUMMY_RADIUS = 0.5;
const double Constants::BIG_MUMMY_SPEED = 2.0;


const double Constants::PHARAOH_RADIUS = 0.45;
const double Constants::PHARAOH_SPEED = 3.3;
const double Constants::PHARAOH_SUMMON_RANGE = 2.0;
const double Constants::PHARAOH_MANA_REGEN = 5.0;

const double Constants::POTION_SPRITE_CENTER_X = 15.0;
const double Constants::POTION_SPRITE_CENTER_Y = 15.0;
const double Constants::POTION_HEIGHT = 16.0;

const std::string Constants::DATA_LOCATION = INSTALL_PATH "/";
const std::string Constants::BINARY_CONFIGURATION_FILENAME = "settings.bin";
const std::string Constants::INI_CONFIGURATION_FILENAME = "settings.ini";
const std::string Constants::BACKGROUND_MUSIC = "musics/bgmusic.wav";
const std::wstring Constants::VERSION = L"v" GAME_VERSION;*/
}
