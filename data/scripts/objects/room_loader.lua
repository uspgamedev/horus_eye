
require "component"
require "context"

function build (wobj, ...)
  local room = ...
  return {
    on_die_callbacks = { function (obj) context.ActivateRoom(room) end }
  }
end
