
require "ugdk.action"
require "ugdk.math"
require "pyramidworks.geometry"
require "component"
require "context"
require "builder"

local Rect = pyramidworks_geometry.Rect

local function make_region()
  local region = {}
  region.activated = false
  return region
end

function build (wobj, params)
  local activated = false

  wobj:set_identifier("Spawn Region #"..params.id)
  wobj:AddComponent(component.Damageable(), "damageable", 0)
  wobj:AddOnRemoveCallback(function (self)
    local n = params.multiple and #params.multiple or 1
    for i=1,n do
      local offset = params.multiple and params.multiple[i] or ugdk_math.Vector2D()
      self:current_room():MakeRecipe(
        params.recipe,
        self:world_position()+offset,
        params.tag or "",
        map.POSITION_ABSOLUTE
      )
    end
  end)
  
  return {
    collision = {
      class = "EventArea",
      shape = params.shapefactory and params.shapefactory() or Rect(1.0, 1.0),
      known_collision = {
        Hero = function(self)
          if not activated then
            activated = true
            print(self:identifier() .. " Activated!")
            if params.delay then
              self:AddComponent(component.TimedLife(tonumber(params.delay)), "timedlife", 0)
            else
              self:damageable():Die()
            end
          end
        end
      }
    },
  }
end
