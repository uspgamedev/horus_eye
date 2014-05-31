
require "ugdk.math"
require "builder"
require "context"
require "constants"
require "event"

local Vector2D = ugdk_math.Vector2D

function build (wobj, params)
  params.spritesheet = params.spritesheet 
  params.life = params.life 
  params.radius = params.radius or             
  params.speed = params.speed 

  builder.PrepareBasicMummy(wobj,
    params.spritesheet    or "mummy-blue",
    params.life           or constants.GetInt "MUMMY_LIFE",
    params.radius         or constants.GetDouble "MUMMY_RADIUS",
    params.speed          or constants.GetDouble "MUMMY_SPEED",
    true,
    "resources/animations/creature.json")

  wobj:animation():ChangeAnimation(component.SPAWNING)
  wobj:caster():power():Set(constants.GetInt "MUMMY_DAMAGE")
  wobj:caster():LearnAndEquipSkill("mummy_melee", component.Controller_PRIMARY)
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
