
require "ugdk.math"
require "builder"
require "context"
require "map"

local Vector2D = ugdk_math.Vector2D

function generate (...)
  local descriptor = {}
  local delay = ...

  print "asdasdasdasd"
  
  descriptor.timed_life = tonumber(delay)

  descriptor.on_die_callback = function (obj)
    local str_args = builder.StringList()
    local mummy = builder.WalkingMummy(str_args)
    obj:current_room():AddObject(mummy, obj:world_position(), map.POSITION_ABSOLUTE)
  end

  return descriptor
end

