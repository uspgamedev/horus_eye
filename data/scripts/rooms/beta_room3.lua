
require "context"
require "component"
require "map"

width = 31
height = 14

arguments = {
    { 23,  2, "damage_switch", "DOOR-3-1", "DOOR-3-2", "DOOR-3-3" },
    { 28,  11, "damage_switch", "DOOR-3-1", "DOOR-3-2", "DOOR-3-3" }
}

tags = {
    { 17, 13, "DOOR-3-1"},
    { 18, 13, "DOOR-3-2"},
    { 19, 13, "DOOR-3-3"},
}

matrix = [[
                 #########     
                 #.M.....#     
                 #.....!.#     
                 #.......#     
                 #.......#     
           #########...########
           #.............#....#
      ######..z.......z..#...M#
..............................#
..L..L..L.....................#
..............................#
      ######..z.......z..#..!.#
           #.............#....#
           ####################
]]

objects = {}

collision_classes = {
  { "Switch", "Wall" },
}

function setup (room)
  print "Setting level up..."
  i = 3
    for j = 1,3 do
      local door = room:WorldObjectByTag("DOOR-"..i.."-"..j)
      if door then
        local damageable = component.Damageable(door)
        damageable:set_life(component.Energy(2))
        door:set_damageable(damageable)
      end
    end
end
