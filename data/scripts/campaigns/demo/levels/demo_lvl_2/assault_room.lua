
require 'ugdk.math'
require 'event'

local vec2 = ugdk_math.Vector2D

event.Clear 'OPEN-ESCAPE-DOOR'

neighborhood = { 'central_room' }

width   = 23
height  = 13

matrix = [[
%%%%%%%%%...%%%%%%%%%
%...................#
%...................#
%..%%..%%...%%..%%..#
%..%....%...%....%..#
%..%....%...%....%..#
%..%....%...%....%..#
%..%....%...%....%..#
%..%%..%%...%%..%%..#
%...................#
%...................#
#####################
]]

recipes = {
  ['entrance-trap'] = {
    property = 'event-region',
    params = {}
  },
  ['escape-button'] = {
    property = 'trigger-button',
    params = { 'OPEN-ESCAPE-DOOR' }
  },
  ['escape-switch'] = {
    property = 'trigger-switch',
    params = { 'OPEN-ESCAPE-DOOR' }
  },
}

function setup (room)
  
end
