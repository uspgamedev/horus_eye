
require "pyramidworks.geometry"
require "component"

local graphics = require 'graphics'

local Rect = pyramidworks_geometry.Rect

function build (wobj)
  local graphic, animator = graphics.switch()
  animator:ChangeAnimation "SWITCH_ON"
  
  wobj:AddComponent(graphic)
  wobj:AddComponent(animator)
  wobj:AddComponent(component.LightEmitter(3.0))
  return {
    collision = {
      class = "Wall",
      shape = Rect(0.8,0.8),
      known_collision = {}
    }
  }
end

