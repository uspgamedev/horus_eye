
module ('event', package.seeall)

local events = {}

--- Registers a new event.
--  Each event trigger has a sequence of events associated to trigger. The
--  order in which the events occur is the order in which they were registered.
--  @param trigger The name of the event.
--  @param callback   A function to be called when the event happens.
function Register (trigger, callback)
  events[trigger] = events[trigger] or {}
  table.insert(events[trigger], callback)
end

--- Clears all events registered under the given trigger.
--  @param trigger The trigger whose events will ve cleared.
function Clear (trigger)
  events[trigger] = nil
end

--- Clears all triggers and associated events.
function ClearAll ()
  events = {}
end

--- Activates an event trigger.
--  All callbacks registered under that trigger will be called.
--  @param trigger The activated trigger.
function Activate (trigger, ...)
  local event_seq = events[trigger]
  if event_seq then
    for _,callback in ipairs(event_seq) do
      callback(...)
    end
  end
end

