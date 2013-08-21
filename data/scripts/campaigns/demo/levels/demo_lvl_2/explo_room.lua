
require "ugdk.math"
require "event"

local vec2 = ugdk_math.Vector2D

neighborhood = { "central_room" }

width = 8
height = 11

matrix = [[
%%%%%%%%
%......%
%.M..M.%
%......%
%.......
%.......
%.......
%......#
%.M..M.#
%......#
########
]]

recipes = {
  ["urn"] = { property = "urn", params = { drops = {"FireballExplosion"} } },
}

function setup (room) 
  for i=1,6 do  
    room:MakeRecipe("urn", vec2(0.75+(i-1), 2.75))
    room:MakeRecipe("urn", vec2(1.25+(i-1), 3.25))
    room:MakeRecipe("urn", vec2(0.75+(i-1), 6.75))
    room:MakeRecipe("urn", vec2(1.25+(i-1), 7.25))
  end
end
