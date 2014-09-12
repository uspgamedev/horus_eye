
require "pyramidworks.geometry"
require "component"

local graphics = require 'graphics'

local Rect = pyramidworks_geometry.Rect

function build (wobj)
  wobj:AddComponent(graphics.switch(), "graphic", 100)
  wobj:graphic():animator():ChangeAnimation "SWITCH_ON"
  wobj:AddComponent(component.LightEmitter(3.0), "light", 101)
  return {
    collision = {
      class = "Wall",
      shape = Rect(0.8,0.8),
      known_collision = {}
    }
  }
end

