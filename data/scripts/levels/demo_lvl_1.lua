require "component"
require "map"
require "constants"

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
  objects = {
    { -1, -1, "!", {"room_loader", "opencorridor"}, "ENTRANCE_ROOM_LOADER" },
  },
  collision_classes = {
    { "Switch", "Wall" }
  },
  setup = function(self)
    (self:WorldObjectByTag "ENTRANCE_ROOM_LOADER"):set_timed_life(0.01)
  end
}

for i = 1,6 do
  local x,y = math.random(2,13), math.random(2,1+(i > 3 and 13+i-3 or i))
  for i = 1,math.random(3,4) do
    table.insert(entrance.objects, {x+math.random()*2-1, y+math.random()*2-1, "!", {"urn"}})
  end
end

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
      { 4, 4, "!", {"room_loader", "closedcorridor"}, "ENTRANCE_ROOM_LOADER" }
   },
   setup = function(self)
     (self:WorldObjectByTag "ENTRANCE_ROOM_LOADER"):set_timed_life(0.01)
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
