
require "ugdk.math"
require "event"

local vec2 = ugdk_math.Vector2D

event.Clear "DIVERGENCE-DOOR-COUNTER"

neighborhood = {
  "entrance", "spike_room", "fireball_room", "central_room"
}

width = 11
height = 16

matrix = [[
%%%%%%%%%%%
%%%..%..%%%
%%.......%%
...........
...........
%%...%...##
%....%....#
%.........#
%%.......##
%.........#
%....%....#
%%...%...##
..........#
..........#
##.......##
####...####
]]

objects = {}

recipes = {
  ["urn"] = { property = "urn" },
  ["open-door"] = {
    property = "open-door",
    params = {
      dir = "LEFT"
    }
  },
  ["closed-door"] = { property = "closed-door", params = { dir = "Right" } },
  ["mummy-spawn"] = {
    property = "spawn_region",
    params = {
      shapefactory = function ()
        return pyramidworks_geometry.Rect(4.0, 4.0)
      end,
      id = 42,
      tag = "Mummy",
      delay = 1.0,
      recipe = "mummy"
    }
  },
  ["mummy"] = {
    property = 'custom-monster',
    params = {
      spritesheet = "mummy-blue",
      life = constants.GetInt "MUMMY_LIFE",
      radius = constants.GetDouble "MUMMY_RADIUS",
      speed = constants.GetDouble "MUMMY_SPEED",
      extra = function (wobj)
        wobj:caster():power():Set(constants.GetInt "MUMMY_DAMAGE")
        wobj:caster():LearnAndEquipSkill("mummy_melee", 0)
      end
    }
  }
}

collision_classes = {
  { "Switch", "Wall" },
  { "Area" },
  { "EventArea" }
}

function setup (self)
  local counter = 2
  event.Register(
    "DIVERGENCE-DOOR-COUNTER",
    function ()
      counter = counter - 1
      if counter <= 0 then
        for i=1,3 do
          self:WorldObjectByTag("THE-LOCKED-DOOR-"..i):Die()
        end
      end
    end
  )
  self:MakeRecipe("mummy-spawn", vec2(5, 3))
  self:MakeRecipe("mummy-spawn", vec2(5, 8))
  self:MakeRecipe("mummy-spawn", vec2(5, 13))
  self:MakeRecipe("urn", vec2(3, 1))
  self:MakeRecipe("urn", vec2(3.5, 1))
  self:MakeRecipe("urn", vec2(4, 1))
  self:MakeRecipe("urn", vec2(6, 1))
  self:MakeRecipe("urn", vec2(6.5, 1))
  self:MakeRecipe("urn", vec2(7, 1))
  self:MakeRecipe("urn", vec2(1, 6))
  self:MakeRecipe("urn", vec2(1, 6.5))
  self:MakeRecipe("urn", vec2(1, 7))
  self:MakeRecipe("urn", vec2(1, 9))
  self:MakeRecipe("urn", vec2(1, 9.5))
  self:MakeRecipe("urn", vec2(1, 10))
  self:MakeRecipe("urn", vec2(9, 6))
  self:MakeRecipe("urn", vec2(9, 6.5))
  self:MakeRecipe("urn", vec2(9, 7))
  self:MakeRecipe("urn", vec2(9, 9))
  self:MakeRecipe("urn", vec2(9, 9.5))
  self:MakeRecipe("urn", vec2(9, 10))
  self:MakeRecipe("urn", vec2(9, 12))
  self:MakeRecipe("urn", vec2(9, 12.5))
  self:MakeRecipe("urn", vec2(9, 13))
  self:MakeRecipe("open-door", vec2(10, 3), "FIREDOOR-1")
  self:MakeRecipe("open-door", vec2(10, 4), "FIREDOOR-2")
  self:MakeRecipe("open-door", vec2(0, 12), "SPIKEDOOR-1")
  self:MakeRecipe("open-door", vec2(0, 13), "SPIKEDOOR-2")
  self:MakeRecipe("closed-door", vec2(4, 15), "THE-LOCKED-DOOR-1")
  self:MakeRecipe("closed-door", vec2(5, 15), "THE-LOCKED-DOOR-2")
  self:MakeRecipe("closed-door", vec2(6, 15), "THE-LOCKED-DOOR-3")
end
