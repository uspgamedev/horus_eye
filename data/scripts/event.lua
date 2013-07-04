
module ('event', package.seeall)

local events = {}

-- NOTE: Each event name has a sequence of events associated to trigger. The
-- order in which the events occur is the order in which they were registered.

function Register (event_name, callback)
  events[event_name] = events[event_name] or {}
  table.insert(events[event_name], callback)
end

function Throw (event_name)
  local event_seq = events[event_name]
  if event_seq then
    for _,callback in ipairs(event_seq) do
      callback()
    end
  end
end

