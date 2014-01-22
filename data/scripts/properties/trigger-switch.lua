
require "ugdk.action"
require "ugdk.drawable"
require "pyramidworks.geometry"
require "component"
require "context"
require "map"

local Sprite    = ugdk_drawable.Sprite
local Rect      = pyramidworks_geometry.Rect

local function make_switch ()
  local switch = proxy "Observer"
  switch.activated = false
  switch.graphic = component.Graphic_Create("switch", "animations/switch.gdd")
  switch.graphic:animator():ChangeAnimation "SWITCH_OFF"
  switch.graphic:animator():AddObserver(switch)
  function switch:Tick ()
    if self.activated then
      self.graphic:animator():ChangeAnimation "SWITCH_ON"
    end
  end
  return switch
end

function build(wobj, params)
  local switch = make_switch()
  wobj:AddComponent(switch.graphic, "graphic", 100)
  
  return {
    collision = {
      class = "Switch",
      shape = Rect(0.8, 0.8),
      known_collision = {
        Projectile = function(self, obj)
          if not switch.activated then
            for _,trigger in pairs(params) do
              event.Activate(trigger)
            end
            switch.activated = true
            switch.graphic:animator():ChangeAnimation "SWITCH_START"
            self:AddComponent(component.Light(3.0), "light", 101)
          end
        end
      }
    }
  }
end
