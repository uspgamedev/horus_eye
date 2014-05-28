
require "pyramidworks.geometry"
require "pyramidworks.collision"
require "ugdk.math"
require "component"
require "context"
require "ugdk.action"

local Circle = pyramidworks_geometry.Circle
local vec2 = ugdk_math.Vector2D

local function AddItemLogic(wobj, callback)

  -- Floating effect
  local logic = proxy "Base"
  logic.period = 2/3*math.pi
  logic.total_time = 0.0
  logic.base_offset = wobj:graphic():render_offset()

  function logic:Update(dt)
    self.total_time = self.total_time + dt
    if self.total_time >= logic.period then
      self.total_time = self.total_time - logic.period
    end
    wobj:graphic():set_render_offset(self.base_offset + vec2(0.0, 0.5*math.cos(3.0*self.total_time)))
  end

  wobj:AddComponent(logic, "item", 0)

  -- Collision logic
  local colobj = pyramidworks_collision.CollisionObject(nil, "Item", Circle(0.15))
  colobj:AddCollisionLogic("Hero", function(other)
    local other_obj = context.ToWorldObject(other:owner())
    if callback(other_obj) then
      wobj:Die()
    end
  end)
  wobj:AddComponent(component.Body(colobj, nil), "body", 0)
end

function build (wobj, params)
  local graphic_comp = component.Graphic_CreateWithSingleFrame("items", "life_potion")
  graphic_comp:set_render_offset(-vec2(13, 24))
  wobj:AddComponent(graphic_comp, "graphic", 100)

  AddItemLogic(wobj, function(other)
    local life = other:damageable():life()
    if life:Get() < life:max_value() then
      life:Increase(constants.GetInt("LIFEPOTION_RECOVER_LIFE"))
      return true
    end
    return false
  end)

  collectgarbage()
end
