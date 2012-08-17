
require "ugdk.drawable"
require "ugdk.action"
require "pyramidworks.geometry"
require "component"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function generate ()
  local args = {}
  local sprite = Sprite("switch", "animations/switch.gdd")
  local observer = proxy "Observer"
  
  observer.activated = false
  function observer:Tick ()
    if self.activated then
      sprite:SelectAnimation "SWITCH_ON"
    end
  end
  
  args.drawable = sprite
  args.drawable:SelectAnimation "SWITCH_OFF"
  args.drawable:AddObserverToAnimation(observer)

  args.collision = {
    class = "Wall",
    shape = Rect(0.8, 0.8),
    known_collision = {},
    custom_collision = {
      Projectile = function(self, obj)
        if not observer.activated then
          for i=1,3 do
            local door = component.WorldObjectByTag("DOOR_"..i)
            if not door then
              print "door not found"
            else
              door:Die()
              observer.activated = true
              sprite:SelectAnimation "SWITCH_START"
            end
          end
        end
      end
    }
  }

  return args
end
