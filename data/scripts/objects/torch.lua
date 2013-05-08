
require "ugdk.drawable"
require "ugdk.graphic"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite

function build (wobj)
  wobj:AddComponent(component.BaseGraphic(Sprite("magic_missile"), 3.0), "graphic", 100)
  wobj:graphic():set_render_offset(ugdk_math.Vector2D(0, -40))
end
