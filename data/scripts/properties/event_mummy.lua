
require "ugdk.math"
require "builder"
require "context"
require "constants"

local Vector2D = ugdk_math.Vector2D

function build (wobj, params)
  builder.PrepareBasicMummy(wobj, params.spritesheet, params.life, params.radius, params.speed, true)
  wobj:caster():power():Set(constants.GetInt "MUMMY_DAMAGE")
  -- FIXME GAMBS MAGIC NUMBER
  -- Should be Controller.PRIMARY
  wobj:caster():LearnAndEquipSkill("mummy_melee", 0)
  return {
    on_die_callbacks = { params.callback }
  }
end

