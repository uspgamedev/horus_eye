
require "ugdk.math"
require "event"

local vec2 = ugdk_math.Vector2D

event.Clear "FALL_SPIKES"

neighborhood = { "divergence" }

width = 9
height = 6

matrix = [[
%%%%%%%%%
%.......%
%........
%........
%.......#
#########
]]

recipes = {
  ["fall-spikes-trigger"] = {
    property = "trigger",
    params = { activates = "FALL_SPIKES", delay = 0.4 }
  },
  ["spike-trap"] = { property = "spike-trap" },
  ["spike-trap-event"] = {
    property = "event-region",
    params = {
      shape = pyramidworks_geometry.Rect(1.0, 4.0),
      callback = function (region)
        local room = region:current_room()
        event.Register(
          "FALL_SPIKES",
          function ()
            room:MakeRecipe("spike-trap", vec2(5, 1))
            room:MakeRecipe("spike-trap", vec2(5, 2))
            room:MakeRecipe("spike-trap", vec2(5, 3))
            room:MakeRecipe("spike-trap", vec2(5, 4))
            event.Clear "FALL_SPIKES"
          end
        )
        room:MakeRecipe "fall-spikes-trigger"
      end
    }
  },
  ["urn"] = { property = "urn" },
  ["door-left"] = { property = "closed-door", params = { dir = "Left" } },
  ["door-right"] = { property = "closed-door", params = { dir = "Right" } },
  ["eventbutton"] = {
    property = "trigger-button",
    params = { "DIVERGENCE-DOOR-COUNTER" }
  }
}

collision_classes = {
  {"Spike"},
  {"EventArea"}
}

function setup (room)
  room:MakeRecipe("eventbutton", vec2(2,2.5))
  room:MakeRecipe("spike-trap-event", vec2(5,2.5))
end

