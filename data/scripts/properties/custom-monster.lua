
require "ugdk.math"
require "builder"
require "context"
require "constants"

local Vector2D = ugdk_math.Vector2D

function build (wobj, params)
  builder.PrepareBasicMummy(
    wobj,
    params.spritesheet,
    params.life,
    params.radius,
    params.speed,
    true,
    params.animations or 'resources/animations/creature.json'
  )
  if params.extra then
    params.extra(wobj)
  end
  if params.callback then
    wobj:AddOnRemoveCallback(params.callback)
  end
end

