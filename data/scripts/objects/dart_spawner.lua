
require "ugdk.math"
require "builder"
require "context"
require "map"

local Vector2D = ugdk_math.Vector2D

function generate (...)
  local descriptor = {}
  local delay, dx ,dy = ...

  descriptor.timed_life = tonumber(delay)

  descriptor.on_die_callback = function (obj)
    local dart =
      builder.ProjectileBuilder():MummyProjectile(Vector2D(dx,dy), 200)
    obj:current_room():AddObject(dart, obj:world_position(), map.POSITION_ABSOLUTE)
  end

  return descriptor
end

