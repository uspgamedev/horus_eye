
require "context"

function generate (...)
  local descriptor = {}
  local room = ...

  descriptor.on_die_callback = function (obj)
    context.ActivateRoom(room)
  end

  return descriptor
end

function build (wobj)
end
