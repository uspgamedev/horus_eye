
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
  switch.graphic = component.Graphic_Create("switch", "animations/switch.gdd")
  switch.graphic:set_render_offset(-ugdk_math.Vector2D(53, 93))
  switch.graphic:animator():ChangeAnimation "SWITCH_OFF"
  switch.graphic:animator():AddObserver(switch)
  function switch:Tick ()
    if self.activated then
      self.graphic:animator():ChangeAnimation "SWITCH_ON"
    end
  end
  return switch
end

function build(wobj, to_kill)
  local switch = make_switch()
  wobj:AddComponent(switch.graphic, "graphic", 100)
  
  return {
    collision = {
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
                switch.graphic:animator():ChangeAnimation "SWITCH_START"
                if not self:HasComponent "light" then
                  self:AddComponent(component.LightEmitter(3.0), "light", 101)
                end
              end
            end
          end
        end
      }
    }
  }
end
