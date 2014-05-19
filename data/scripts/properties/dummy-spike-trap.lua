
require "ugdk.drawable"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function build (wobj)
  local graphic_comp = component.Graphic_Create("dummy-spike-trap", "animations/spike-trap.gdd")
  graphic_comp:set_render_offset(-ugdk_math.Vector2D(53, 135))
  graphic_comp:animator():ChangeAnimation "STANDING"
  wobj:AddComponent(graphic_comp, "graphic", 100)
  return {
    collision = {
      class = "Wall",
      shape = Rect(1,1),
      known_collision = {}
    }
  }
end
