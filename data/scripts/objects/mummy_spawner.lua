
require "ugdk.math"
require "builder"
require "context"
require "map"

local Vector2D = ugdk_math.Vector2D

function build (wobj, ...)
  local delay = ...
  wobj:set_timed_life(tonumber(delay))
  return {
    on_die_callback = function (obj)
      local str_args = builder.StringList()
      local mummy = builder.WalkingMummy(str_args)
      obj:current_room():AddObject(mummy, obj:world_position(), map.POSITION_ABSOLUTE)
    end
  }
end

