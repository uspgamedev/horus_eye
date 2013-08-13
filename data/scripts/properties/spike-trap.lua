
require "ugdk.drawable"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function build (wobj)
  local graph_comp = component.Graphic("dummy-spike-trap", "animations/spike-trap.gdd")
  wobj:AddComponent(graph_comp, "graphic", 100)
  wobj:set_timed_life(1)
  local anim_comp = component.Animation(wobj, component.SPAWNING, component.Direction())
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
