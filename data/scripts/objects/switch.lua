
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
  switch.graphic = component.Graphic("switch", "animations/switch.gdd")
  switch.graphic:ChangeAnimation "SWITCH_OFF"
  switch.graphic:animation_player():AddObserver(switch)
  function switch:Tick ()
    if self.activated then
      self.graphic:ChangeAnimation "SWITCH_ON"
    end
  end
  return switch
end

function build (wobj)
  local switch = make_switch()
  wobj:AddComponent(switch.graphic, "graphic", 100)

  return {
    collision = {
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
              end
            end
            switch.activated = true
            switch.graphic:ChangeAnimation "SWITCH_START"
            self:graphic():ChangeLightRadius(3.0)
          end
        end
      }
    }
  }
end
