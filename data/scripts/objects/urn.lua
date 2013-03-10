
require "ugdk.drawable"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Circle = pyramidworks_geometry.Circle

function generate ()
  local descriptor = {
    drawable = Sprite("vase"),
    collision = {
      class = "Wall",
      shape = Circle(0.15),
      known_collision = {
        Projectile = function(self, obj) self:Die() end
      }
    }
  }
  return descriptor
end

function build (wobj)
  wobj:node():geometry():set_scale(ugdk_math.Vector2D(0.225, 0.225))
end
