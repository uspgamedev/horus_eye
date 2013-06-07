require "component"
require "map"

music = "musics/Arabesque.ogg"
roomsize = 16
rooms = {
   {0, roomsize * 0, "entrance"},
   {roomsize * 1, 5, "opencorridor"},
   {0, roomsize * 2, "closedcorridor"},
   {0, roomsize * 3, "firstblood"},
   {0, roomsize * 4, "exit"}
}

start_position = {"entrance", roomsize / 2, roomsize / 2}

width = roomsize*#rooms
height = roomsize

entrance = {
  width = roomsize,
  height = roomsize,
  matrix = [[
################
#..............#
#..............#
#..............#
#..............#
#..............#
#...............
#...............
#...............
#...............
#..............#
#..............#
#..............#
#..............#
#..............#
################
]],
  objects = {
    { 4, 4, "!", {"room_loader", "opencorridor"}, "ENTRANCE_ROOM_LOADER" },
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
................
................
................
................
################
]],
}