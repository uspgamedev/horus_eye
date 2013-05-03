
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

function generate (w,h,x,y, spawner_type, ...)
  local descriptor = {}
  local trap = make_trap()
  local args = { ... }

  descriptor.collision = {
    class = "EventArea",
    shape = Rect(w, h),
    known_collision = {
      Hero = function(self, obj)
        if not trap.activated then
          local str_args = builder.StringList()
          str_args:push_back(spawner_type)
          --str_args:push_back "0.2"
          for _,arg in pairs(args) do
            str_args:push_back(arg)
          end
          local spawner = builder.Script(str_args)
      	  self:current_room():AddObject(spawner, Vector2D(x,y))
          trap.activated = true
          print(self:identifier() .. " Activated!")
        end
      end
    }
  }

  return descriptor
end

i = 1
function build (wobj)
	wobj:set_identifier("Spawn Trap #"..i)
	i = i + 1
end
