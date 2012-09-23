
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
    local function place_life(tag, hp)
		local obj = room:WorldObjectByTag(tag)
		if obj then
			local damageable = component.Damageable(obj)
			damageable:set_life(component.Energy(hp))
			obj:set_damageable(damageable)
		else
			print("No object with tag '"..tag.."'!")
		end
    end
    i=2
    for j = 1,3 do
		place_life("DOOR-"..i.."-"..j, 2)
    end
	place_life("ROOM-"..i.."-LOADER", 2)
end
