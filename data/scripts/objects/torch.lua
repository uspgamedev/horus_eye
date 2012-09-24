
require "ugdk.drawable"
require "ugdk.graphic"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite

function generate ()
  local descriptor = {
    drawable = Sprite("magic_missile")
  }
  return descriptor
end

function build (wobj)
  wobj:set_light_radius(3.0)
  wobj:node():drawable():set_hotspot(ugdk_math.Vector2D(0, 40))
end
