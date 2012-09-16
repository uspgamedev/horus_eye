
require "context"
require "component"
require "map"

width = 15
height = 9

arguments = {
    { 11,  4, "kill_switch",   "DOOR-1-1", "DOOR-1-2", "DOOR-1-3" },
}

tags = {
    { 14,  3, "DOOR-1-1"},
    { 14,  4, "DOOR-1-2"},
    { 14,  5, "DOOR-1-3"},
}

matrix = [[
###############
#.............#
#.............#
#.............#
E.@........!..#
#.............#
#.............#
#.............#
###############
]]

collision_classes = {
  { "Switch", "Wall" }
}
