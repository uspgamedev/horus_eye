
require "ugdk.drawable"
require "ugdk.action"
require "pyramidworks.geometry"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

local function make_switch ()
  local switch = proxy "Observer"
  switch.activated = false
  switch.sprite = Sprite("switch", "animations/switch.gdd")
  local player = switch.sprite:animation_player()
  player:Select "SWITCH_OFF"
  player:AddObserver(switch)
  function switch:Tick ()
    if self.activated then
      self.sprite:animation_player():Select "SWITCH_ON"
    end
  end
  return switch
end

function generate ()
  local descriptor = {}
  local switch = make_switch()
  
  descriptor.drawable = switch.sprite

  descriptor.collision = {
    class = "Switch",
    shape = Rect(0.8, 0.8),
    known_collision = {
      Projectile = function(self, obj)
        if not switch.activated then
          for i=1,3 do
            local door = self:current_room():WorldObjectByTag("DOOR_"..i)
            if not door then
              print "door not found"
            else
              door:damageable():TakeDamage(1)
              switch.activated = true
              switch.sprite:animation_player():Select "SWITCH_START"
            end
          end
        end
      end
    }
  }

  return descriptor
end

function build (wobj)
end
