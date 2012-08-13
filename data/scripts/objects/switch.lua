
require "ugdk.drawable"
require "pyramidworks.geometry"
require "component"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function generate ()
  local args = {}
  local activated = false
  args.drawable = Sprite("eye", "animations/switch.gdd")
  args.drawable:SelectAnimation "SWITCH_ON"

  args.collision = {
    class = "Wall",
    shape = Rect(1.0, 1.0),
    known_collision = {},
    custom_collision = {
      Projectile = function(self, obj)
        if not activated then
          for i=1,3 do
            local door = component.WorldObjectByTag("DOOR_"..i)
            if not door then
              print "door not found"
            else
              door:Die()
              activated = true
            end
          end
        end
      end
    }
  }

  return args
end
