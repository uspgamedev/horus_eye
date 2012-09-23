
require "context"
require "component"
require "map"

width = 25
height = 15

arguments = {
    { 21,  2, "damage_switch", "DOOR-2-1", "DOOR-2-2", "DOOR-2-3", "ROOM-2-LOADER" },
    { 21, 12, "damage_switch", "DOOR-2-1", "DOOR-2-2", "DOOR-2-3", "ROOM-2-LOADER" },
    { 17,  7, "room_loader", "beta_room3" },
}

tags = {
    { 18,  6, "DOOR-2-1"},
    { 18,  7, "DOOR-2-2"},
    { 18,  8, "DOOR-2-3"},
    { 17,  7, "ROOM-2-LOADER"},
}

matrix = [[
#########################
#..............#........#
#....................!..#
 .......................#
 ..............#........#
 ..............##########
 .................#      
 ................!#      
 .................#      
 ..............##########
 ..............#........#
 .......................#
#....................!..#
#..............#........#
#########################
]]

objects = {
  { 16, 11.5, "!", "spawn_trap", "17", "10", "dart_spawner", "0", "1" },
  {  2,  6.5, "!", "spawn_trap", "11", "7", "mummy_spawner" },
}

collision_classes = {
  { "Switch", "Wall" },
  { "EventArea" }
}

function setup (room)
    i=2
    for j = 1,3 do
        context.AddDamageableComponent(room, "DOOR-"..i.."-"..j, 2)
    end
    context.AddDamageableComponent(room, "ROOM-"..i.."-LOADER", 2)
end
