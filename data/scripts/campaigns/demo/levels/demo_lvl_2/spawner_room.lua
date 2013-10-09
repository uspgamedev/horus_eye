
require "ugdk.math"
require "event"

local vec2 = ugdk_math.Vector2D

neighborhood = {
  "central_room"
}

width = 18
height = 13

matrix = [[
%%%%%%%%%%%%%%%%%%
%%%%%%%..........#
%%%%.............#
%%...............#
%%............%%%%
..................
..................
..................
%%............%%%%
%%...............#
%%%%.............#
%%%%%%%..........#
%%%%%%%###########
]]

recipes = {
  ["open-door"] = {
    property = "open-door",
    params = {
      dir = "LEFT",
    }
  }
}

function setup (room)
  room:MakeRecipe("open-door", vec2(14, 5))
  room:MakeRecipe("open-door", vec2(14, 6))
  room:MakeRecipe("open-door", vec2(14, 7))
end
