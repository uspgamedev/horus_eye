
require "context"
require "component"
require "map"

width = 15
height = 9

arguments = {
    { 11,  4, "kill_switch", "DOOR-1-1", "DOOR-1-2", "DOOR-1-3", "ROOM-1-LOADER" },
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

objects = {
    { -1, -1, "!", { "room_loader", "beta_room2" }, "ROOM-1-LOADER" },
    { 13.5,  2, "!", { "torch" } },
    { 13.5,  6, "!", { "torch" } },
    { 8.5,  7.25, "!", { "urn" } },
}

collision_classes = {
  { "Switch", "Wall" }
}
