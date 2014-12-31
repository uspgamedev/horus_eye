
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local graphics = require 'graphics'

local Rect = pyramidworks_geometry.Rect

function build (wobj, params)
  local dir = params.dir or "Left"

  wobj:AddComponent(component.Damageable())
  
  local graphic, animator = graphics.closed_door()
  wobj:AddComponent(graphic)
  wobj:AddComponent(animator)
  
  local anim_comp = component.Animation()
  wobj:AddComponent(anim_comp)
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
