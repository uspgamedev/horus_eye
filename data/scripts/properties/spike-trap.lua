
require "ugdk.drawable"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function build (wobj)
  local graph_comp = component.Graphic_Create("dummy-spike-trap", "animations/spike-trap.gdd")
  graph_comp:set_render_offset(-ugdk_math.Vector2D(53, 135))
  wobj:AddComponent(graph_comp, "graphic", 100)
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
