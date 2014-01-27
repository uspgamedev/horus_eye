
require "ugdk.drawable"
require "pyramidworks.geometry"
require "component"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function build (wobj)
  local graphic = component.Graphic_Create("switch", "animations/switch.gdd")
  graphic:animator():ChangeAnimation "SWITCH_ON"
  wobj:AddComponent(graphic, "graphic", 100)
  wobj:AddComponent(component.LightEmitter(3.0), "light", 101)
  return {
    collision = {
      class = "Wall",
      shape = Rect(0.8,0.8),
      known_collision = {}
    }
  }
end

