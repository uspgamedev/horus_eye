
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"
require "builder"
require "map"

local graphics = require 'graphics'

local Circle = pyramidworks_geometry.Circle
local vec2 = ugdk_math.Vector2D

function build (wobj, params)
  params = params or {}
  local graphic, animator = graphics.urn()
  wobj:AddComponent(graphic, "graphic", 100)
  wobj:AddComponent(animator, "animator", 102)
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
