
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local graphics = require 'graphics'

local Rect = pyramidworks_geometry.Rect

function build (wobj, params)
  local dir = params.dir or "Left"

  wobj:AddComponent(component.Damageable(), "damageable", 0)
  
  local graphic, animator = graphics.closed_door()
  wobj:AddComponent(graphic, "graphic", 100)
  wobj:AddComponent(animator, "animator", 102)
  
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
