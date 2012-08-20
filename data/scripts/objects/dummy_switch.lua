
require "ugdk.drawable"
require "ugdk.action"
require "pyramidworks.geometry"
require "component"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

local function make_switch ()
  local switch = proxy "Observer"
  switch.activated = false
  switch.sprite = Sprite("switch", "animations/switch.gdd")
  switch.sprite:SelectAnimation "SWITCH_OFF"
  switch.sprite:AddObserverToAnimation(switch)
  function switch:Tick ()
    if self.activated then
      self.sprite:SelectAnimation "SWITCH_ON"
    end
  end
  return switch
end

function generate ()
  local descriptor = {}
  local switch = make_switch()
  
  descriptor.drawable = switch.sprite

  descriptor.collision = {
    class = "Wall",
    shape = Rect(0.8, 0.8),
    known_collision = {},
    custom_collision = {
      Projectile = function(self, obj)
        if not switch.activated then
          switch.activated = true
          switch.sprite:SelectAnimation "SWITCH_START"
        end
      end
    }
  }

  return descriptor
end

