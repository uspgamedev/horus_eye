
require "ugdk.drawable"
require "ugdk.action"
require "ugdk.math"
require "pyramidworks.geometry"
require "component"
require "context"
require "builder"

local Rect = pyramidworks_geometry.Rect

local function make_region()
  local region = {}
  region.activated = false
  return region
end

function build (wobj, params)
  local activated = false

  wobj:set_identifier("Spawn Region #"..params.id)
  
  return {
    collision = {
      class = "EventArea",
      shape = params.shapefactory and params.shapefactory() or Rect(1.0, 1.0),
      known_collision = {
        Hero = function(self)
          if not activated then
            activated = true
            print(self:identifier() .. " Activated!")
            if params.delay then
              self:set_timed_life(tonumber(params.delay))
            else
              self:Die()
            end
          end
        end
      }
    },
    on_die_callbacks = {
      function (self)
        local n = params.multiple and #params.multiple or 1
        for i=1,n do
          local offset =
            params.multiple and params.multiple[i] or ugdk_math.Vector2D()
          self:current_room():MakeRecipe(
            params.recipe,
            self:world_position()+offset,
            params.tag or "",
            map.POSITION_ABSOLUTE
          )
        end
      end
    }
  }
end