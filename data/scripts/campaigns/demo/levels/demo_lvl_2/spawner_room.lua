
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
      spritesheet = 'altar',
      life = constants.GetInt 'CURSED_ALTAR_LIFE',
      radius = 1.2,
      speed = 0,
      animations = 'altar',
      extra = function (wobj)
        wobj:caster():LearnAndEquipSkill("altar_summon", 0)
        -- TODO THIS
        -- Remember: CollisionObjects steal geometry's ownership!
        --wobj:body():AddCollision(pyramidworks_collision.CollisionObject(wobj, 'Wall', rect(1,1)))
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
