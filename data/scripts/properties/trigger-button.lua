
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
  button.graphic = component.Graphic_CreateWithAnimationSet("scenery", "resources/animations/button.json")
  button.graphic:set_render_offset(-ugdk_math.Vector2D(53, 27))
  button.graphic:animator():ChangeAnimation "OFF"
  button.graphic:set_layer(component.BACKGROUND_LAYER)
  button.activated = false
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
            button.activated = true
            for _,trigger in pairs(params) do
              event.Activate(trigger)
            end
            button.graphic:animator():ChangeAnimation "ON"
          end
        end
      }
    }
  }
end
