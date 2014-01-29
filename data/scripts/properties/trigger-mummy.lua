
require "ugdk.math"
require "builder"
require "context"
require "constants"
require "event"

local Vector2D = ugdk_math.Vector2D

function build (wobj, params)
  params.spritesheet = params.spritesheet or "mummy_basic"
  params.life = params.life or constants.GetInt "MUMMY_LIFE"
  params.radius = params.radius or constants.GetDouble "MUMMY_RADIUS"
  params.speed = params.speed or constants.GetDouble "MUMMY_SPEED"
  builder.PrepareBasicMummy(wobj, params.spritesheet, params.life, params.radius, params.speed, true)
  wobj:caster():power():Set(constants.GetInt "MUMMY_DAMAGE")
  -- FIXME GAMBS MAGIC NUMBER
  -- Should be Controller.PRIMARY
  wobj:caster():LearnAndEquipSkill("mummy_melee", 0)
  return {
    on_die_callbacks = {
      function (self)
        for _,trigger in ipairs(params.triggers) do
          event.Activate(trigger)
        end
      end
    }
  }
end

