
require "ugdk.drawable"
require "ugdk.action"
require "ugdk.math"
require "pyramidworks.geometry"
require "component"
require "context"
require "builder"
require "event"

local function make_region()
  local region = {}
  region.activated = false
  return region
end

i = 1
function build (wobj, params)
  local region = make_region()

  wobj:set_identifier("Spawn Region #"..i)
  i = i + 1

  return {
    collision = {
      class = "EventArea",
      shape = params.shape,
      known_collision = {
        Hero = function(self, obj)
          if not region.activated then
            region.activated = true
            print(self:identifier() .. " Activated!") -- TODO: Debug print
            for _,trigger in ipairs(params) do
              event.Activate(trigger)
            end
          end
        end
      }
    }
  }
end
