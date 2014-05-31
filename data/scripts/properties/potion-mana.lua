
require "ugdk.math"
require "component"

local commonpotion = require 'properties.common.potion'

function build (wobj, params)
  local graphic_comp = component.Graphic_CreateWithSingleFrame("items", "mana_potion")
  graphic_comp:set_render_offset(-ugdk_math.Vector2D(13, 24))
  wobj:AddComponent(graphic_comp, "graphic", 100)

  commonpotion.AddItemLogic(wobj, function(other)
    local caster = other:caster()
    if caster and not caster:mana():Full() then
      caster:mana():Increase(constants.GetInt("MANAPOTION_RECOVER_MANA"))
      return true
    end
    return false
  end)
end
