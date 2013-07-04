
require "ugdk.math"
require "builder"
require "context"
require "constants"

local Vector2D = ugdk_math.Vector2D

function build (wobj, spritesheet, life, radius, speed, ...)
  local triggers = {...}
  builder.PrepareBasicMummy(wobj, spritesheet, life, radius, speed, true)
  wobj:caster():power():Set(constants.GetInt "MUMMY_DAMAGE")
  -- FIXME GAMBS MAGIC NUMBER
  -- Should be Controller.PRIMARY
  wobj:caster():LearnAndEquipSkill("mummy_melee", 0)
  return {
    on_die_callbacks = {
      function (self)
        for _,trigger in ipairs(triggers) do
          local triggerobj = self:current_room():WorldObjectByTag(trigger)
          if triggerobj and triggerobj:damageable() then
            triggerobj:damageable():TakeDamage(1)
          end
        end
      end
    }
  }
end

