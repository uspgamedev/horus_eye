
require "context"
require "map"
require "event"

function build (wobj, params)
  local activates = params.activates or "UNKNOWN_TRIGGER" 
  if params.delay then
    wobj:AddComponent(component.TimedLife(tonumber(params.delay)))
  end
  wobj:AddOnRemoveCallback(function (self)
    event.Activate(activates)
  end)
end

