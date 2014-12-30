
require "context"
require "map"
require "event"

function build (wobj, params)
  local activates = params.activates or "UNKNOWN_TRIGGER" 
  if params.delay then
    wobj:AddComponent(component.TimedLife(tonumber(params.delay)), "timedlife", 0)
  end
  return {
    on_die_callbacks = {
      function (obj)
        event.Activate(activates)
      end
    }
  }
end

