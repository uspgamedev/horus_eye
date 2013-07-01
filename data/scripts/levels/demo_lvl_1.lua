require "component"
require "map"

music = "musics/Arabesque.ogg"
roomsize = 16
rooms = {
   {roomsize * 0, 0, "entrance"},
   {roomsize * 1, 5, "opencorridor"},
   {roomsize * 2, 5, "closedcorridor"},
   {roomsize * 3, 0, "firstblood"},
   --{0, roomsize * 4, "exit"}
}

start_position = {"entrance", roomsize / 2, roomsize / 2}

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
...............#
...............#
...............#
...............#
%..............#
%..............#
%..............#
%..............#
%..............#
################
]],
  objects = {
    {
      2,  roomsize/2, --position
      "!", --scriptobj
      {
        "spawn_trap", --type
        1.0, roomsize, -- width, height
        roomsize-3, roomsize/2, -- position
        "mummy_spawner", 0.5 -- spawned object
      }
    },
  },
  collision_classes = {
    { "EventArea" }
  },
}
