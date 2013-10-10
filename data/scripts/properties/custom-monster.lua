
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
    params.animations or 'creature'
  )
  if params.extra then
    params.extra(wobj)
  end
  return {
    on_die_callbacks = { params.callback }
  }
end

