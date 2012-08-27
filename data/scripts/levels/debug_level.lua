
require "context"
require "component"

music = "musics/Arabesque.ogg"
width = 15
height = 34
arguments = {
    { 4, 1, "spike2" },
    { 4, 2, "spike" },
    { 4, 3, "spike" },
    { 3, 11, "switch" },
    { 11, 11, "switch" }
}
tags = {
    { 4, 3, "EXPLO" },
    { 7, 13, "DOOR_1"},
    { 8, 13, "DOOR_2"},
    { 9, 13, "DOOR_3"}
}
matrix = [[
###########E###
#..z!UUUUUU@# #
#..z!UUUUUU.# #
#..z!UUUUUU.# #
#.UUU.DD.UU.###
#.UUUUUUUUU...#
#.UUUUUUUUU...#
#.............#
#.............#
#...K.........#
#.............#
#..!.......!..#
#.............#
###############
#.........M...#
#.............#
#.............#
#.............#
#.............#
#########...###
#.......#.K.###
#.......#...###
#.#######...###
#.#...........#
#.#..L........#
#.#..M........#
#.#..M........#
#.#..M........#
#.#....U......#
#.#...........#
#.#....DD.....#
#.#.L.........#
#.............#
###############
]]

objects = {
    { 7, 32, "!", "dummy_switch" },
}

collision_classes = {
  { "Switch", "Wall" }
}

function setup ()
  print "=========================================="
  for i = 1,3 do
    local door = context.WorldObjectByTag("DOOR_"..i)
    local damageable = component.Damageable(door)
    damageable:set_life(component.Energy(2))
    door:set_damageable(damageable)
  end
end
