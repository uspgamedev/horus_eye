
require "ugdk.drawable"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function build (wobj, params)
  local dir = params.dir or "Left"
  local graphic_comp = component.Graphic_Create("closed-door", "animations/closed-door.gdd")
  wobj:AddComponent(graphic_comp, "graphic", 100)
  local anim_comp = component.Animation(wobj:get())
  wobj:AddComponent(anim_comp, "animation", 101)
  anim_comp:ChangeDirection(component["Direction_"..dir]())
  context.EnableDeathAnimation(wobj:get())
  return {
    collision = {
      class = "Wall",
      shape = Rect(1,1),
      known_collision = {}
    }
  }
end
