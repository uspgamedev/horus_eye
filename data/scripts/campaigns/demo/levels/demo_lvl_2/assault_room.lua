
require 'ugdk.math'
require 'pyramidworks.geometry'
require 'event'

local vec2 = ugdk_math.Vector2D
local rect = pyramidworks_geometry.Rect

event.Clear 'OPEN-ESCAPE-DOOR'

neighborhood = { 'central_room' }

width   = 23
height  = 13

matrix = [[
%%%%%%%%%%...%%%%%%%%%%
%.....................#
%.....................#
%..%%..%%.....%%..%%..#
%..%....%.....%....%..#
%..%....%.....%....%..#
%..%....%.....%....%..#
%..%....%.....%....%..#
%..%%..%%.....%%..%%..#
%.....................#
%.....................#
#######################
]]

recipes = {
  ['entrance-trap'] = {
    property = 'event-region',
    params = {
      shape = rect(5, 1),
      callback = function (self)
        local room = self:current_room()
        room:MakeRecipe('door', vec2(10, 0), 'THE-DOOR-1')
        room:MakeRecipe('door', vec2(11, 0), 'THE-DOOR-2')
        room:MakeRecipe('door', vec2(12, 0), 'THE-DOOR-3')
      end
    }
  },
  ['door'] = { property = 'closed-door', params = { dir = 'Right' } },
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
  room:MakeRecipe('entrance-trap', vec2(11, 1.5))
end
