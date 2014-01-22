
require "ugdk.drawable"
require "ugdk.action"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function build (wobj, params)
  local dir = params and params.dir or "LEFT"
  local open_event = params and params.open_event
  local close_event = params and params.close_event

  local graphic_comp = component.Graphic_Create("closed-door", "animations/closed-door.gdd")
  wobj:AddComponent(graphic_comp, "graphic", 100)
  graphic_comp:animator():ChangeAnimation("STANDING_"..dir)

  local observer = proxy "Observer"
  graphic_comp:animator():animation_player():AddObserver(observer)
  observer.state = 'down'
  function observer:Tick ()
    if self.state == 'up'  then
      self.state = 'closing'
      graphic_comp:animator():ChangeAnimation("SPAWNING_"..dir)
      if close_event then close_event() end
    elseif self.state == 'opening' then
      self.state = 'up'
      graphic_comp:animator():ChangeAnimation("NONE")
    elseif self.state == 'closing' then
      self.state = 'down'
      graphic_comp:animator():ChangeAnimation("STANDING_"..dir)
    end
  end

  return {
    collision = {
      class = "Area",
      shape = Rect(3,3),
      known_collision = {
        Creature = function (self, creature)
          if observer.state == 'down' then
            observer.state = 'opening'
            graphic_comp:animator():ChangeAnimation("DYING_"..dir)
            if open_event then open_event() end
          elseif observer.state == 'closing' then
            observer.state = 'up'
            graphic_comp:animator():ChangeAnimation("NONE")
          end
        end
      }
    }
  }
end

