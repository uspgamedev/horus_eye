
require "ugdk.math"
require "event"

local vec2 = ugdk_math.Vector2D

local function DOOR (i)
  return "THE-DOOR-"..i
end

event.Clear "DOOR-COUNTER"

neighborhood = { "divergence" }

width = 11
height = 12

matrix = [[
%%%%%%%%%%%
%%%.......%
%%%.......%
%.........%
%.......%%%
%..........
%..........
%.......%%%
%.........%
%%%.......%
%%%.......%
%%%%%%%%%%%
]]

recipes = {
  ["urn"] = { property = "urn" },
  ["door"] = { property = "closed-door", params = { dir = "Left" } },
  ["door-switch"] = {
    property = "trigger-switch",
    params = { "DOOR-COUNTER" }
  },
}

collision_classes = {
  { "Switch", "Wall" }
}

function setup (self)
  local switch_counter = 2
  event.Register(
    "DOOR-COUNTER",
    function ()
      switch_counter = switch_counter - 1
      if switch_counter <= 0 then
        for i=1,2 do
          self:WorldObjectByTag(DOOR(i)):Die()
        end
      end
    end
  )
  self:MakeRecipe("door", vec2(10, 5), DOOR(1))
  self:MakeRecipe("door", vec2(10, 6), DOOR(2))
  self:MakeRecipe("door-switch", vec2(9, 2))
  self:MakeRecipe("door-switch", vec2(9, 9))
  for i = 1,2 do
    local x,y = 4.5, 1.2+8.6*(i-1)
    for i = 1,6 do
      self:MakeRecipe("urn", vec2(x+0.6*(i-1), y))
    end
  end
end
