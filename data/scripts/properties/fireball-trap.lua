
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"
require "builder"

local Rect = pyramidworks_geometry.Rect

function build (wobj, params)
  local graph_comp = component.Graphic_CreateWithSingleFrame("projectile", "green-ball")
  graph_comp:set_render_offset(ugdk_math.Vector2D(0, -58) - ugdk_math.Vector2D(16, 16))
  wobj:AddComponent(graph_comp)
  wobj:AddComponent(component.TimedLife(1))
  local dir = params.dir
  if not dir then
    dir = ugdk_math.Vector2D(0,0)
  end
  builder.PrepareProjectile(wobj, dir, 8)
  --local anim_comp = component.Animation(component.SPAWNING, component.Direction())
  --wobj:AddComponent(anim_comp)
  --context.EnableDeathAnimation(wobj)
  return {
    collision = {
      class = "Projectile",
      shape = Rect(0.2,0.2),
      known_collision = {
        Hero = function (self, obj)
          obj:damageable():TakeDamage(100)
        end
      }
    }
  }
end
