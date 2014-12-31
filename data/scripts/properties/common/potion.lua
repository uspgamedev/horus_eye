
require "pyramidworks.geometry"
require "pyramidworks.collision"
require "ugdk.math"
require "component"
require "context"

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
    local other_obj = context.ToWorldObject(other:data())
    if callback(other_obj) then
      wobj:Remove()
    end
  end)
  wobj:AddComponent(component.Body(colobj, nil), "body", 0)
end

return {
  AddItemLogic = AddItemLogic
}