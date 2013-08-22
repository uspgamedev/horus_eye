
require "ugdk.math"
require "event"

local vec2 = ugdk_math.Vector2D

neighborhood = { "divergence" }

width = 9
height = 6

matrix = [[
%%%%%%%%%
%.......%
........%
........%
#.......%
####&####
]]

recipes = {
  ["fireball-trap"] = {
    property = "fireball-trap",
    params = {dir = vec2(0, -1)}
  },
  ["fireball-event"] = {
    property = "event_region",
    params = {
      shape = pyramidworks_geometry.Rect(1.0, 4.0),
      callback = function (region)
        local room = region:current_room()
        room:MakeRecipe("fireball-trap", vec2(4, 5))
      end
    }
  },
  ["eventbutton"] = {
    property = "trigger-button",
    params = { "DIVERGENCE-DOOR-COUNTER" }
  }
}

collision_classes = {
  { "EventArea" }
}

function setup (room)
  room:MakeRecipe("eventbutton", vec2(4,2.5))
  room:MakeRecipe("fireball-event", vec2(4,2.5))
end
