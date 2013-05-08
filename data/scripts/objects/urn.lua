
require "ugdk.drawable"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Circle = pyramidworks_geometry.Circle

function build (wobj)
  wobj:AddComponent(component.BaseGraphic(Sprite("vase")), "graphic", 100)
  return {
    collision = {
      class = "Wall",
      shape = Circle(0.15),
      known_collision = {
        Projectile = function(self, obj) self:Die() end
      }
    }
  }
end
