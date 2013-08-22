
require "component"
require "map"
require "constants"
require "ugdk.math"
require "pyramidworks.geometry"
require "event"

local roomsize = 12
local vec2 = ugdk_math.Vector2D

music = "musics/Arabesque.ogg"
rooms = {
  {0, 0, "entrance"},
  {11, 2, "divergence"},
  {2, 12, "spike_room"},
  {22, 3, "fireball_room"},
  {8, 18, "central_room"},
  {0, 21, "explo_room"},
  {5, 35, "assault_room"}
}

start_position = {"entrance", 2, roomsize / 2}

width = 100
height = 100
