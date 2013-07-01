
require "component"
require "context"

function build (wobj, room, time, unload)
  local event
  if unload then
    event = function (obj) context.DeactivateRoom(room) end
  else
    event = function (obj) context.ActivateRoom(room) end
  end
  if time then
    wobj:set_timed_life(tonumber(time))
  end
  return {
    on_die_callbacks = {
      event
    }
  }
end
