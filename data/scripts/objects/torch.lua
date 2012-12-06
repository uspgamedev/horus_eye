
require "ugdk.drawable"
require "ugdk.graphic"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite

function generate ()
  local descriptor = {
    drawable = Sprite("magic_missile"),
    light_radius = 3.0,
  }
  return descriptor
end

function build (wobj)
  wobj:graphic():set_render_offset(ugdk_math.Vector2D(0, -40))
end
