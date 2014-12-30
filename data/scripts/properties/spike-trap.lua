
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local graphics = require 'graphics'

local Rect = pyramidworks_geometry.Rect

function build (wobj)
  local graphic, animator = graphics.spike_trap()
  wobj:AddComponent(graphic, "graphic", 100)
  wobj:AddComponent(animator, "animator", 102)
  wobj:set_timed_life(1)
  local anim_comp = component.Animation(component.SPAWNING, component.Direction())
  wobj:AddComponent(anim_comp, "animation", 101)
  context.EnableDeathAnimation(wobj)
  return {
    collision = {
      class = "Spike",
      shape = Rect(0.9,0.9),
      known_collision = {
        Hero = function (self, obj)
          obj:damageable():TakeDamage(100)
        end
      }
    }
  }
end
