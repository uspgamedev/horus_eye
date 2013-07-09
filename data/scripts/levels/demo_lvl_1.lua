require "component"
require "map"
require "constants"
require "ugdk.math"

music = "musics/Arabesque.ogg"
roomsize = 16
rooms = {
   {roomsize * 0, 0, "entrance"},
   {roomsize * 1, 5, "opencorridor"},
   {roomsize * 2, 5, "closedcorridor"},
   {roomsize * 3, 0, "firstblood"},
   {roomsize * 4, 5, "exit"}
}

start_position = {"entrance", roomsize / 2 - 5, roomsize / 2}

width = roomsize*#rooms
height = roomsize

entrance = {
  width = roomsize,
  height = roomsize,
  matrix = [[
%%%%%%%%%%%%%%%%
%..............%
%..............%
%..............%
%..............%
%..............%
%...............
%...............
%...............
%...............
%..............#
%..............#
%..............#
%..............#
%..............#
################
]],
  objects = {},
  recipes = {
    load_corridor = { property = "room_loader", params = { room = "opencorridor", time = 0.01 } },
    urn = { property = "urn" },
  },
  setup = function(self)
    self:MakeRecipe "load_corridor"
    for i = 1,6 do
      local x,y = math.random(2,13), math.random(2,1+(i > 3 and 13+i-3 or i))
      for i = 1,math.random(3,4) do
        self:MakeRecipe("urn", ugdk_math.Vector2D(x+math.random()*2-1, y+math.random()*2-1))
      end
    end
  end
}
 
opencorridor = {
  width = roomsize,
  height = roomsize - 10,
  matrix = [[
%%%%%%%%%%%%%%%%
...............%
................
................
...............#
#######&########
]],
  objects = {
    {  12,   3, "!", { "activated_switch" } },
    { 8 + math.random(), 1 + math.random() * 3, "!", { "dummy-spike-trap" } },
    { 4 + math.random(), 1 + math.random() * 3, "!", { "dummy-spike-trap" } },
  },
  recipes = {
    load_corridor = { property = "room_loader", params = { room = "closedcorridor", time = 0.01 } },
  },
  setup = function(self)
  end
}

closedcorridor = {
  width = roomsize,
  height = roomsize - 10,
  matrix = [[
%%%%%%%%%%%%%%%%
...............%
................
................
...............#
#######&########
]],
  objects = {
    { 8 + math.random(), 1 + math.random() * 3, "!", { "dummy-spike-trap" } },
    { 4 + math.random(), 1 + math.random() * 3, "!", { "dummy-spike-trap" } },
    { 15, 2, "!", {"closed-door", "LEFT"}, "THE-DOOR-1" },
    { 15, 3, "!", {"closed-door", "LEFT"}, "THE-DOOR-2" },
    { 12, 2, "!", {"kill_switch", "THE-DOOR-1", "THE-DOOR-2", "CORRIDOR_ROOM_LOADER"} },
    { -1, -1, "!", {"room_loader", "firstblood"}, "CORRIDOR_ROOM_LOADER" },
  }
}

local function spawn_region (x,y,...)
  return {
    3,  roomsize/2, --position
    "!", --scriptobj
    {
      "spawn_trap", --type
      1.0, roomsize, -- width, height
      x, y, -- position
      ... -- spawned object
    }
  }
end

firstblood = {
  width = roomsize,
  height = roomsize,
  matrix = [[
%%%%%%%%%%%%%%%%
%..............#
%..............#
%..............#
%..............#
%..............#
%..............#
................
................
%..............#
%..............#
%..............#
%..............#
%..............#
%..............#
################
]],
  objects = {
    spawn_region(
      roomsize-3, 3,
      "custom_mummy_spawner",
      0.5,
      "event_mummy",
      "mummy_basic",
      constants.GetInt "MUMMY_LIFE",
      constants.GetDouble "MUMMY_RADIUS",
      constants.GetDouble "MUMMY_SPEED",
      "THE-DOOR-1",
      "THE-DOOR-2",
      "BATTLE_ROOM_LOADER"
    ),
    spawn_region(
      roomsize-3, roomsize-3,
      "custom_mummy_spawner",
      0.5,
      "event_mummy",
      "mummy_basic",
      constants.GetInt "MUMMY_LIFE",
      constants.GetDouble "MUMMY_RADIUS",
      constants.GetDouble "MUMMY_SPEED",
      "THE-DOOR-1",
      "THE-DOOR-2",
      "BATTLE_ROOM_LOADER"
    ),
    spawn_region(0, 7, "closed-door", "LEFT"),
    spawn_region(0, 8, "closed-door", "LEFT"),
    spawn_region(-1, -1, "room_loader", "closedcorridor", 0, "true"),
    { 15, 7, "!", {"closed-door", "LEFT"}, "THE-DOOR-1" },
    { 15, 8, "!", {"closed-door", "LEFT"}, "THE-DOOR-2" },
    { -1, -1, "!", {"room_loader", "exit"}, "BATTLE_ROOM_LOADER" },
  },
  collision_classes = {
    { "EventArea" }
  },
  setup = function (room)
    for i=1,2 do
      context.AddDamageableComponent(room, "THE-DOOR-"..i, 2)
    end
    context.AddDamageableComponent(room, "BATTLE_ROOM_LOADER", 2)
  end
}

exit = {
   width = roomsize,
   height = roomsize - 10,
   matrix = [[
%%%%%%%%%%%%%%%#
...............#
...........D...#
...............#
...............#
################
]],
   objects = {
   },
   setup = function(self)
   end
}

