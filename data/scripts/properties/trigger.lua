
require "context"
require "map"
require "data.scripts.event"

function build (wobj, params)
  local activates = params.activates or "UNKNOWN_TRIGGER" 
  if params.delay then
    wobj:set_timed_life(tonumber(params.delay))
  end
  return {
    on_die_callbacks = {
      function (obj)
        event.Activate(activates)
      end
    }
  }
end

