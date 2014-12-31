
require "ugdk.action"
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local graphics = require 'graphics'

local Rect = pyramidworks_geometry.Rect

function build (wobj, params)
  local dir = params and params.dir or "LEFT"
  local open_event = params and params.open_event
  local close_event = params and params.close_event

  local graphic_comp, animator = graphics.open_door()
  wobj:AddComponent(graphic_comp)
  wobj:AddComponent(animator)

  animator:ChangeAnimation("STANDING_"..dir)

  local observer = proxy "Observer"
  animator:AddObserver(observer)
  observer.state = 'down'
  function observer:Tick ()
    if self.state == 'up'  then
      self.state = 'closing'
      animator:ChangeAnimation("SPAWNING_"..dir)
      if close_event then close_event() end
    elseif self.state == 'opening' then
      self.state = 'up'
      animator:ChangeAnimation("NONE")
    elseif self.state == 'closing' then
      self.state = 'down'
      animator:ChangeAnimation("STANDING_"..dir)
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
            animator:ChangeAnimation("DYING_"..dir)
            if open_event then open_event() end
          elseif observer.state == 'closing' then
            observer.state = 'up'
            animator:ChangeAnimation("NONE")
          end
        end
      }
    }
  }
end

