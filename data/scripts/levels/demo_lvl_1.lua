music = "music/Arabesque.ogg"
roomsize = 16
rooms = {
   {0, roomsize * 0, "entrance"},
   {0, roomsize * 1, "opencorridor"},
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
   }
   
}

for i = 1,10 do
   table.insert(entrance.objects, {math.random(2,15), math.random(2,15), "!", {"urn"}})
end