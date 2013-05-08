
require "context"

function build (wobj, ...)
  local room = ...
  return {
    on_die_callback = function (obj) context.ActivateRoom(room) end
  }
end
