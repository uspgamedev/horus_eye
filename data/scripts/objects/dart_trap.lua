
require "ugdk.drawable"
require "ugdk.action"
require "ugdk.math"
require "pyramidworks.geometry"
require "component"
require "context"
require "builder"

local Vector2D  = ugdk_math.Vector2D
local Sprite    = ugdk_drawable.Sprite
local Rect      = pyramidworks_geometry.Rect
local function make_trap ()
  local trap = {}
  trap.activated = false
  return trap
end

function generate (...)
  local descriptor = {}
  local trap = make_trap()
  local x,y,dx,dy = ...

  descriptor.collision = {
    class = "EventArea",
    shape = Rect(1.0, 2.0),
    known_collision = {},
    custom_collision = {
      Hero = function(self, obj)
        if not trap.activated then
          local args = builder.StringList()
          args:push_back "dart_spawner"
          args:push_back "0.2"
          args:push_back(dx)
          args:push_back(dy)
          local spawner =
            builder.Script(args)
          context.AddWorldObject(spawner, Vector2D(x,y))
          trap.activated = true
        end
      end
    }
  }

  return descriptor
end

function build (wobj)
end
