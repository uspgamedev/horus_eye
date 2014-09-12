
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local graphics = require 'graphics'

local Rect = pyramidworks_geometry.Rect

function build (wobj, params)
  local dir = params.dir or "Left"
  wobj:AddComponent(graphics.closed_door(), "graphic", 100)
  
  local anim_comp = component.Animation()
  wobj:AddComponent(anim_comp, "animation", 101)
  anim_comp:ChangeDirection(component["Direction_"..dir]())
  context.EnableDeathAnimation(wobj)
  return {
    collision = {
      class = "Wall",
      shape = Rect(1,1),
      known_collision = {}
    }
  }
end
