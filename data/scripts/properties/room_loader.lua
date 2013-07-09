
require "component"
require "context"

function build (wobj, params)
  -- room, time, unload
  local event
  if params.unload then
    event = function (obj) context.DeactivateRoom(params.room) end
  else
    event = function (obj) context.ActivateRoom(params.room) end
  end
  if params.time then
    wobj:set_timed_life(tonumber(params.time))
  end
  return {
    on_die_callbacks = {
      event
    }
  }
end
