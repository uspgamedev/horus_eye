
require "ugdk.math"
require "builder"
require "context"
require "component"
require "map"

local Vector2D = ugdk_math.Vector2D

function build (wobj, params)
  wobj:AddComponent(component.TimedLife(tonumber(params.time)), "timedlife", 0)
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

