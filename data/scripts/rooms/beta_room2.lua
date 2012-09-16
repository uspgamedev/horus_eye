
require "context"
require "component"
require "map"

width = 25
height = 15

arguments = {
    { 21,  2, "damage_switch", "DOOR-2-1", "DOOR-2-2", "DOOR-2-3" },
    { 21, 12, "damage_switch", "DOOR-2-1", "DOOR-2-2", "DOOR-2-3" },
}

tags = {
    { 18,  6, "DOOR-2-1"},
    { 18,  7, "DOOR-2-2"},
    { 18,  8, "DOOR-2-3"},
}

matrix = [[
#########################
#..............#........#
#....................!..#
 .......................#
 ..............#........#
 ..............##########
 .................#      
 .........M.......#      
 .................#      
 ..............##########
 ..............#........#
 .......................#
#....................!..#
#..............#........#
#########################
]]

objects = {
  { 16, 11.5, "!", "dart_trap", "17", "10", "0", "1" }
}

collision_classes = {
  { "Switch", "Wall" },
  { "EventArea" }
}

function setup (room)
    print "Setting level up..."
    i=2
    for j = 1,3 do
      local door = room:WorldObjectByTag("DOOR-"..i.."-"..j)
      if door then
        local damageable = component.Damageable(door)
        damageable:set_life(component.Energy(2))
        door:set_damageable(damageable)
      end
    end
end
