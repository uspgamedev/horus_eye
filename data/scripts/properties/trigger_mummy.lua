
require "ugdk.math"
require "builder"
require "context"
require "constants"

local Vector2D = ugdk_math.Vector2D

function build (wobj, params)
  params.spritesheet = params.spritesheet or "mummy-blue"
  params.life = params.life or constants.GetInt "MUMMY_LIFE"
  params.radius = params.radius or constants.GetDouble "MUMMY_RADIUS"
  params.speed = params.speed or constants.GetDouble "MUMMY_SPEED"
  builder.PrepareBasicMummy(wobj, params.spritesheet, params.life, params.radius, params.speed, true,
    "resources/animations/creature.json")
  wobj:caster():power():Set(constants.GetInt "MUMMY_DAMAGE")
  -- FIXME GAMBS MAGIC NUMBER
  -- Should be Controller.PRIMARY
  wobj:caster():LearnAndEquipSkill("mummy_melee", 0)
  wobj:AddOnRemoveCallback(function (self)
    for _,trigger in ipairs(params.triggers) do
      local triggerobj = self:current_room():WorldObjectByTag(trigger)
      if triggerobj and triggerobj:damageable() then
        triggerobj:damageable():TakeDamage(1)
      end
    end
  end)
end

