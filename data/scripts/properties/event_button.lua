
require "ugdk.action"
require "ugdk.drawable"
require "ugdk.action"
require "pyramidworks.geometry"
require "component"
require "context"
require "map"

local Sprite    = ugdk_drawable.Sprite
local Rect      = pyramidworks_geometry.Rect

local function make_button ()
  local button = proxy "Observer"
  --button.activated = false
  button.graphic = component.Graphic("button", "animations/button.gdd")
  button.graphic:ChangeAnimation "OFF"
  button.graphic:set_layer(component.BACKGROUND_LAYER)
  --button.graphic:animation_player():AddObserver(button)
  --function button:Tick ()
  --  if self.activated then
  --    self.graphic:ChangeAnimation "ON"
  --  end
  --end
  return button
end

function build(wobj, params)
  local button = make_button()
  wobj:AddComponent(button.graphic, "graphic", 100)
  
  return {
    collision = {
      class = "Button",
      shape = Rect(1, 1),
      known_collision = {
        Hero = function(self, obj)
          if not button.activated then
            for _,trigger in pairs(params) do
              event.Activate(trigger)
            end
            --button.activated = true
            button.graphic:ChangeAnimation "ON"
            --button.graphic:ChangeLightRadius(3.0)
          end
        end
      }
    }
  }
end
