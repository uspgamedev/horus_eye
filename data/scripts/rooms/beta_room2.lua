
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
  {  2,  6.5, "!", "spawn_trap", "15", "7", "mummy_spawner" },
}

collision_classes = {
  { "Switch", "Wall" },
  { "EventArea" }
}

function setup (room)
    print "Setting level up..."
    i=2
    local function place_life(obj, hp)
      local damageable = component.Damageable(obj)
      damageable:set_life(component.Energy(hp))
      obj:set_damageable(damageable)
    end
    for j = 1,3 do
      local door = room:WorldObjectByTag("DOOR-"..i.."-"..j)
      if door then
        place_life(door, 2)
      end
    end
    local loader = room:WorldObjectByTag("ROOM-2-LOADER")
    if loader then
      place_life(loader, 2)
    end
end
