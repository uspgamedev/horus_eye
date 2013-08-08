
require "ugdk.action"
require "ugdk.drawable"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

local function make_spike ()
  local spike = proxy "Observer"
  spike.fallen = false
  spike.graphic = component.Graphic("dummy-spike-trap", "animations/spike-trap.gdd")
  spike.graphic:ChangeAnimation "FALLING"
  spike.graphic:animation_player():AddObserver(spike)
  function spike:Tick ()
    if not self.fallen then
      self.graphic:ChangeAnimation "STANDING"
      self.fallen = true
    end
  end
  return spike
end

function build (wobj)
  local spike = make_spike()
  wobj:AddComponent(spike.graphic, "graphic", 100)
  return {
    collision = {
      class = "Spike",
      shape = Rect(0.9,0.9),
      known_collision = {
        Hero = function (self, obj)
          obj:damageable():TakeDamage(100)
        end
      }
    }
  }
end
