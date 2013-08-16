
require "ugdk.drawable"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"
require "builder"

local Sprite = ugdk_drawable.Sprite
local Circle = pyramidworks_geometry.Circle
local vec2 = ugdk_math.Vector2D

function build (wobj, params)
  local graphic_comp = component.Graphic("vase", "animations/urn.gdd")
  params = params or {}
  graphic_comp:ChangeAnimation "STANDING"
  wobj:AddComponent(graphic_comp, "graphic", 100)
  return {
    collision = {
      class = "Wall",
      shape = Circle(0.15),
      known_collision = {
        Projectile = function(self, obj) self:Die() end,
        Explosion = function(self, obj) self:Die() end
      }
    },
    on_die_callbacks = {
      function (self)
        local drops = params.drops or {}
        local offset = params.offset or vec2()
        for _,drop in ipairs(drops) do
          local builtin = builder[drop]
          if builtin then
            self:current_room():AddObject(
              builtin(),
              self:world_position()+offset,
              map.POSITION_ABSOLUTE
            )
          else
            self:current_room():MakeRecipe(
              drop,
              self:world_position()+offset,
              params.tag or "",
              map.POSITION_ABSOLUTE
            )
          end
        end
      end
    }
  }
end
