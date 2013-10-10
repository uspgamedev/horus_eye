
require "ugdk.math"
require "event"

local vec2 = ugdk_math.Vector2D

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
      radius = 2,
      speed = 0,
      animations = 'altar',
      extra = function (wobj)
        --wobj:caster():LearnAndEquipSkill("altar_summon", 0)
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
