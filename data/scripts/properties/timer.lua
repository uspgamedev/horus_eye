
require "context"
require "map"
require "data.scripts.event"

function build (wobj, params)
  local delay = params.time
  wobj:set_timed_life(tonumber(delay))
  return {
    on_die_callbacks = {
      function (obj)
        event.Activate(obj:tag())
      end
    }
  }
end

