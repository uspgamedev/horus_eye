
require "ugdk.math"
require "builder"
require "context"
require "map"

local Vector2D = ugdk_math.Vector2D

function build (wobj, params)
  wobj:set_timed_life(tonumber(params.time))
  return {
    on_die_callbacks = {
      function (obj)
        obj:current_room():MakeRecipe(
          params.recipe,
          obj:world_position(),
          params.tag or "", map.POSITION_ABSOLUTE
        )
      end
    }
  }
end

