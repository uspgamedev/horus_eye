
require "context"
require "component"
require "map"

width = 31
height = 14

arguments = {
    { 23,  2, "damage_switch", "DOOR-3-1", "DOOR-3-2", "DOOR-3-3", "ROOM-3-LOADER" },
    { 28, 11, "damage_switch", "DOOR-3-1", "DOOR-3-2", "DOOR-3-3", "ROOM-3-LOADER" },
    { 18, 12, "room_loader", "beta_room4" },
}

tags = {
    { 17, 13, "DOOR-3-1"},
    { 18, 13, "DOOR-3-2"},
    { 19, 13, "DOOR-3-3"},
    { 18, 12, "ROOM-3-LOADER"},
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
           #......!......#....#
           ####################
]]

collision_classes = {
    { "Switch", "Wall" },
}

function setup (room)
    i=3
    for j = 1,3 do
        context.AddDamageableComponent(room, "DOOR-"..i.."-"..j, 2)
    end
    context.AddDamageableComponent(room, "ROOM-"..i.."-LOADER", 2)
end
