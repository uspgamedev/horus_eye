
require "ugdk.drawable"
require "ugdk.action"
require "ugdk.math"
require "pyramidworks.geometry"
require "component"
require "context"
require "builder"

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
      shape = params.shape,
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
        self:current_room():MakeRecipe(
          params.recipe,
          self:world_position(),
          params.tag or "",
          map.POSITION_ABSOLUTE
        )
      end
    }
  }
end
