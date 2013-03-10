
require "ugdk.action"
require "ugdk.drawable"
require "ugdk.action"
require "pyramidworks.geometry"
require "component"
require "context"
require "map"

local Sprite    = ugdk_drawable.Sprite
local Rect      = pyramidworks_geometry.Rect

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

function generate (...)
  local descriptor = {}
  local switch = make_switch()
  local to_kill = { ... }

  descriptor.drawable = switch.sprite

  descriptor.collision = {
    class = "Switch",
    shape = Rect(0.8, 0.8),
    known_collision = {
      Projectile = function(self, obj)
        if not switch.activated then
          for _,tag in pairs(to_kill) do
            local door = self:current_room():WorldObjectByTag(tag)
            if not door then
              print("Tag "..tag.." not found")
            else
              door:Die()
              switch.activated = true
			  switch.sprite:animation_player():Select "SWITCH_START"
              self:graphic():ChangeLightRadius(3.0)
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
