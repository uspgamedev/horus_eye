
require "context"
require "component"

music = "musics/Arabesque.ogg"
width = 64
height = 26

arguments = {
    { 11,  9, "kill_switch",   "DOOR-1-1", "DOOR-1-2", "DOOR-1-3" },
    { 35,  4, "damage_switch", "DOOR-2-1", "DOOR-2-2", "DOOR-2-3" },
    { 35, 14, "damage_switch", "DOOR-2-1", "DOOR-2-2", "DOOR-2-3" },
    { 56,  2, "damage_switch", "DOOR-3-1", "DOOR-3-2", "DOOR-3-3" },
    { 61,  11, "damage_switch", "DOOR-3-1", "DOOR-3-2", "DOOR-3-3" }
}

tags = {
    { 14,  8, "DOOR-1-1"},
    { 14,  9, "DOOR-1-2"},
    { 14, 10, "DOOR-1-3"},
    { 32,  8, "DOOR-2-1"},
    { 32,  9, "DOOR-2-2"},
    { 32, 10, "DOOR-2-3"},
    { 50, 13, "DOOR-3-1"},
    { 51, 13, "DOOR-3-2"},
    { 52, 13, "DOOR-3-3"},
}

matrix = [[
                                                  #########     
                                                  #.M.....#     
              #########################           #.....!.#     
              #..............#........#           #.......#     
              #....................!..#           #.......#     
###############.......................#     #########...########
#.............#..............#........#     #.............#....#
#.............#..............################..z.......z..#...M#
#.............#.................#..............................#
E.@........!..#.........M.......#..L..L..L.....................#
#.............#.................#..............................#
#.............#..............################..z.......z..#..!.#
#.............#..............#........#     #.............#....#
###############.......................#     ####################
              #....................!..#     #..................#
              #..............#........#     #.M..............M.#
              #########################     #..................#
                                            #.z..............z.#
                                            #..................#
                                            #.z..............z.#
                                            #..................#
                                            #.z..............z.#
                                            #........DD........#
                                            #.M..............M.#
                                            #..................#
                                            ####################
]]

objects = {
  { 29, 11.5, "!", "spawn_trap", "30", "13", "dart_spawner", "0", "-1" }
}

collision_classes = {
  { "Switch", "Wall" },
  { "EventArea" }
}

function setup ()
  print "Setting level up..."
  for i = 2,3 do
    for j = 1,3 do
      local door = context.WorldObjectByTag("DOOR-"..i.."-"..j)
      if door then
        local damageable = component.Damageable(door)
        damageable:set_life(component.Energy(2))
        door:set_damageable(damageable)
      end
    end
  end
end
