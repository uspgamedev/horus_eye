
require 'ugdk.math'
require 'pyramidworks.geometry'
require 'component'
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
        room:MakeRecipe 'assault-spawner'
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
  ['assault-spawner'] = {
    property = 'spawner',
    params = {
      delay = 2,
      recipe = 'paper-mummy',
      points = { vec2(1,1), vec2(21,1), vec2(1,11), vec2(21,11) }
    }
  }
}

function setup (room)
  local device  = math.random() > 0.5 and 'button' or 'switch'
  local side    = math.random() > 0.5 and 5.5 or 16.5
  room:MakeRecipe('entrance-trap', vec2(11, 2))
  room:MakeRecipe('escape-'..device, vec2(side, 5.5))
  event.Register(
    'OPEN-ESCAPE-DOOR',
    function ()
      for i=1,3 do
        room:WorldObjectByTag('THE-DOOR-'..i):Die()
      end
    end
  )
end
