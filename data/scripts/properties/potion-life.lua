
require "ugdk.math"
require "component"

local commonpotion = require 'properties.common.potion'

function build (wobj, params)
  local graphic_comp = component.Graphic_CreateWithSingleFrame("items", "life_potion")
  graphic_comp:set_render_offset(-ugdk_math.Vector2D(13, 24))
  wobj:AddComponent(graphic_comp, "graphic", 100)

  commonpotion.AddItemLogic(wobj, function(other)
    local life = other:damageable():life()
    if not life:Full() then
      life:Increase(constants.GetInt("LIFEPOTION_RECOVER_LIFE"))
      return true
    end
    return false
  end)

  collectgarbage()
end
