
require "context"
require "map"
require "data.scripts.event"

function build (wobj, delay)
  wobj:set_timed_life(tonumber(delay))
  return {
    on_die_callbacks = {
      function (obj)
        event.Throw(obj:tag())
      end
    }
  }
end

