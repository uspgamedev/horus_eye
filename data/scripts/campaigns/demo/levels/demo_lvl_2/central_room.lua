
require "ugdk.math"
require "event"

local vec2 = ugdk_math.Vector2D

local function dual_spawner(offset)
  return {
    property = "spawn_region",
    params = {
      shapefactory = function ()
        return pyramidworks_geometry.Rect(2.0, 2.0)
      end,
      id = 64,
      tag = "Mummy",
      recipe = "mummy",
      multiple = {
        offset,
        -offset,
      }
    }
  }
end

neighborhood = {
  "divergence", "explo_room", "assault_room", "spawner_room"
}

width = 17
height = 17

matrix = [[
%%%%%%%...%%%%%%%
%%%%%%%...%%%%%%%
%%%%.........%%%%
%%%%.........%%%%
%%.............%%
%%....%...%....%%
%%...%%...%%...%%
.................
.................
......M...M......
%%...%%...%%...##
%%....%...%....##
%%.............##
%%%%.........####
%%%%..M...M..####
%%%%%%%...#######
%%%%%%%...#######
]]

recipes = {
  ["horizontal-mummy-spawn"] = dual_spawner(vec2(4, 0)),
  ["vertical-mummy-spawn"] = dual_spawner(vec2(0, 4)),
  ["mummy"] = {
    property = 'custom-monster',
    params = {
      spritesheet = "mummy-blue",
      life = constants.GetInt "MUMMY_LIFE",
      radius = constants.GetDouble "MUMMY_RADIUS",
      speed = constants.GetDouble "MUMMY_SPEED",
      extra = function (wobj)
        wobj:animation():ChangeAnimation(component.SPAWNING)
        wobj:caster():power():Set(constants.GetInt "MUMMY_DAMAGE")
        wobj:caster():LearnAndEquipSkill("mummy_melee", component.Controller_PRIMARY)
      end
    }
  },
  ["open-door"] = {
    property = "open-door",
    params = {
      dir = "LEFT",
    }
  },
}

function setup (room)
  room:MakeRecipe("horizontal-mummy-spawn", vec2(8, 5.5))
  room:MakeRecipe("horizontal-mummy-spawn", vec2(8, 16-5.5))
  room:MakeRecipe("vertical-mummy-spawn", vec2(5.5, 8))
  room:MakeRecipe("vertical-mummy-spawn", vec2(16-5.5, 8))
  room:MakeRecipe("open-door", vec2(0, 7))
  room:MakeRecipe("open-door", vec2(0, 8))
  room:MakeRecipe("open-door", vec2(0, 9))
  room:MakeRecipe("open-door", vec2(16, 7))
  room:MakeRecipe("open-door", vec2(16, 8))
  room:MakeRecipe("open-door", vec2(16, 9))
end
