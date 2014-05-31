
require "context"
require "ugdk.math"
require "event"
require "pyramidworks.collision"
require "pyramidworks.geometry"

local vec2 = ugdk_math.Vector2D
local rect = pyramidworks_geometry.Rect

neighborhood = {
  "central_room"
}

width = 18
height = 13

matrix = [[
%%%%%%%%%%%%%%%%%%
%%%%%%%..........#
%%%%.............#
%%...............#
%%............%%%%
..................
..................
..................
%%............%%%%
%%...............#
%%%%.............#
%%%%%%%..........#
%%%%%%%###########
]]

recipes = {
  ['open-door'] = {
    property = 'open-door',
    params = {
      dir = 'LEFT'
    }
  },
  ['cursed-altar'] = {
    property = 'custom-monster',
    params = {
      spritesheet = 'scenery',
      life = constants.GetInt 'CURSED_ALTAR_LIFE',
      radius = 1.5,
      speed = 0,
      animations = 'resources/animations/altar.json',
      extra = function (wobj)
        wobj:caster():LearnAndEquipSkill("altar_summon", 0)
        context.AddCollisionObjectRect(wobj, 'Wall', 2, 2)
        wobj:body():main_collision():RemoveCollisionLogic('Wall')
      end,
      callback = function (wobj)
        print "IT DIED"
      end
    }
  }
}

function setup (room)
  room:MakeRecipe('open-door', vec2(14, 5))
  room:MakeRecipe('open-door', vec2(14, 6))
  room:MakeRecipe('open-door', vec2(14, 7))
  room:MakeRecipe('cursed-altar', vec2(8, 6))
end
