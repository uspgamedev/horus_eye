
require "ugdk.math"
require "ugdk.drawable"
require "ugdk.action"
require "pyramidworks.geometry"
require "component"
require "builder"

local Vector2D  = ugdk_math.Vector2D
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

function build(wobj)
  local descriptor = {}
  local switch = make_switch()
  
  wobj:AddComponent(component.BaseGraphic(switch.sprite), "graphic", 100)

  return { 
    collision = {
      class = "Wall",
      shape = Rect(0.8, 0.8),
      known_collision = {
        Projectile = function(self, obj)
          if not switch.activated then
            switch.activated = true
            switch.sprite:animation_player():Select "SWITCH_START"
            self:current_room():AddObject(builder.Script("spike"), Vector2D(4, 23))
          end
        end
      }
    }
  }
end

