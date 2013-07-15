
require "context"
require "map"
require "data.scripts.event"

function build (wobj, params)
  local activates = params.activates or "UNKNOWN_TRIGGER" 
  wobj:set_timed_life(tonumber(params.delay) or 0.0)
  return {
    on_die_callbacks = {
      function (obj)
        event.Activate(activates)
      end
    }
  }
end

