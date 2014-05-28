
require 'ugdk.graphic'
require 'ugdk.resource'
require 'ugdk.math'

require "component"

local graphics = {}

function graphics.urn()
  local graphic = component.Graphic_CreateWithAnimationSet("scenery", "resources/animations/urn.json")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 93))
  graphic:animator():ChangeAnimation "STANDING"
  return graphic
end

function graphics.switch()
  local graphic = component.Graphic_CreateWithAnimationSet("switch", "resources/animations/switch.json")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 93))
  graphic:animator():ChangeAnimation "SWITCH_OFF"
  return graphic
end

function graphics.spike_trap()
  local graphic = component.Graphic_CreateWithAnimationSet("scenery", "resources/animations/spike-trap.json")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 135))
  return graphic
end

function graphics.dummy_spike_trap()
  local graphic = component.Graphic_CreateWithSingleFrame("scenery", "dummy-spike-trap")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 135))
  return graphic
end

function graphics.closed_door()
  local graphic = component.Graphic_CreateWithAnimationSet("scenery", "resources/animations/closed-door.json")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 156))
  return graphic
end

function graphics.open_door()
  local graphic = component.Graphic_CreateWithAnimationSet("scenery", "resources/animations/closed-door.json")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 156))
  return graphic
end

return graphics