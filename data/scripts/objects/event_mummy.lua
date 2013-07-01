
require "ugdk.math"
require "builder"
require "context"
require "constants"

local Vector2D = ugdk_math.Vector2D

function build (wobj, spritesheet, life, radius, speed, eventtrigger)
  builder.PrepareBasicMummy(wobj, spritesheet, life, radius, speed, true)
  local caster = wobj:caster()
  local power = caster:power()
  power:Set(constants.GetInt "MUMMY_DAMAGE")
  -- FIXME GAMBS MAGIC NUMBER
  -- Should be Controller.PRIMARY
  wobj:caster():LearnAndEquipSkill("mummy_melee", 0)
  return {
    on_die_callbacks = {
      function (self)
        local triggerobj = self:current_room():WorldObjectByTag(eventtrigger or "")
        if triggerobj and triggerobj:damageable() then
          triggerobj:damageable():TakeDamage(1)
        end
      end
    }
  }
end

