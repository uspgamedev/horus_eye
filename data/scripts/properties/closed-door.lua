
require "ugdk.drawable"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function build (wobj, params)
  local graphic_comp = component.Graphic("closed-door", "animations/closed-door.gdd")
  graphic_comp:ChangeAnimation("STANDING"..(params.dir and ("_"..params.dir) or ""))
  wobj:AddComponent(graphic_comp, "graphic", 100)
  wobj:AddComponent(component.Animation(wobj), "animation", 101)
  context.EnableDeathAnimation(wobj)
  return {
    collision = {
      class = "Wall",
      shape = Rect(1,1),
      known_collision = {}
    }
  }
end
