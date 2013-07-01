
require "ugdk.math"
require "builder"
require "context"
require "map"

local Vector2D = ugdk_math.Vector2D

function build (wobj, delay, script, ...)
  local args = {...}
  wobj:set_timed_life(tonumber(delay))
  return {
    on_die_callbacks = {
      function (obj)
        local str_args = builder.StringList()
        str_args:push_back(script)
        for _,arg in ipairs(args) do
          str_args:push_back(arg)
        end
        local mummy = builder.Script(str_args)
        obj:current_room():AddObject(mummy, obj:world_position(), map.POSITION_ABSOLUTE)
      end
    }
  }
end

