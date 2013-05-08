
require "ugdk.math"
require "builder"
require "context"
require "map"

local Vector2D = ugdk_math.Vector2D

function build(wobj, ...)
  local descriptor = {}
  local delay, dx ,dy = ...

  wobj:set_timed_life(tonumber(delay))
  
  return {
    on_die_callback = function (obj)
      local dart =
        builder.MummyProjectile(Vector2D(dx,dy), 200)
      obj:current_room():AddObject(dart, obj:world_position(), map.POSITION_ABSOLUTE)
    end
  }
end

