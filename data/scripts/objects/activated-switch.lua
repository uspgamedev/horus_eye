
require "ugdk.drawable"
require "ugdk.action"
require "pyramidworks.geometry"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function build (wobj)
  local graphic = component.Graphic("switch", "animations/switch.gdd")
  graphic:ChangeAnimation "SWITCH_ON"
  graphic:ChangeLightRadius(3.0)
  wobj:AddComponent(graphic, "graphic", 100)
  return {
    class = "Switch",
    shape = Rect(0.8,0.8),
    known_collision = {}
  }
end

