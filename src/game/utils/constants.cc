//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/constants.cc
// Implementacao da classe Constants.
//

#include "constants.h"

namespace utils {

const float Constants::DOOR_HOTSPOT_WIDTH = 153.0f/2.0f;
const float Constants::DOOR_HOTSPOT_HEIGHT = 109.0f-(91.0f/2.0f);
const float Constants::DOOR_BOUND_WIDTH = 2.0f;
const float Constants::DOOR_BOUND_HEIGHT = 1.0f;

const float Constants::FLOOR_HOTSPOT_X = 106.0f/2.0f;
const float Constants::FLOOR_HOTSPOT_Y =  61.0f/2.0f;

const float Constants::WALL_HOTSPOT_X = 106.0f/2.0f;
const float Constants::WALL_HOTSPOT_Y = 157.0f - 30.5f;

const float Constants::LIFE_BAR_WIDTH = 200.0f;
const float Constants::LIFE_BAR_HEIGHT = 15.0f;
const float Constants::MANA_BAR_WIDTH = 200.0f;
const float Constants::MANA_BAR_HEIGHT = 15.0f;

const float Constants::LIGHT_RADIUS_THRESHOLD = 0.25f;

const float Constants::HERO_SPEED = 3.0f;

const float Constants::PROJECTILE_SPEED = 8.0f;
const float Constants::PROJECTILE_SPRITE_CENTER_X = 16.0f;
const float Constants::PROJECTILE_SPRITE_CENTER_Y = 16.0f;
const float Constants::PROJECTILE_HEIGHT = 12.0f;

const float Constants::MUMMY_RADIUS = 0.3f;
const float Constants::MUMMY_SPEED = 2.0f;

const float Constants::BIG_MUMMY_RADIUS = 0.5f;
const float Constants::BIG_MUMMY_SPEED = 1.7f;

const float Constants::POTION_SPRITE_CENTER_X = 15.0f;
const float Constants::POTION_SPRITE_CENTER_Y = 15.0f;
const float Constants::POTION_HEIGHT = 16.0f;
}
