
require 'ugdk.graphic'
require 'ugdk.resource'
require 'ugdk.math'

require "component"

local graphics = {}

function graphics.urn()
  local graphic = component.Graphic_CreateWithSpritesheet("scenery")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 93))

  local animator = component.Animator "resources/animations/urn.json"
  animator:ChangeAnimation "STANDING"
  return graphic, animator
end

function graphics.switch()
  local graphic = component.Graphic_CreateWithSpritesheet("switch")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 93))

  local animator = component.Animator "resources/animations/switch.json"
  animator:ChangeAnimation "SWITCH_OFF"
  return graphic, animator
end

function graphics.spike_trap()
  local graphic = component.Graphic_CreateWithSpritesheet("scenery")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 135))

  local animator = component.Animator "resources/animations/spike-trap.json"
  return graphic, animator
end

function graphics.dummy_spike_trap()
  local graphic = component.Graphic_CreateWithSingleFrame("scenery", "dummy-spike-trap")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 135))
  return graphic
end

function graphics.closed_door()
  local graphic = component.Graphic_CreateWithSpritesheet("scenery")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 156))

  local animator = component.Animator "resources/animations/closed-door.json"
  return graphic, animator
end

function graphics.open_door()
  local graphic = component.Graphic_CreateWithSpritesheet("scenery")
  graphic:set_render_offset(-ugdk_math.Vector2D(53, 156))

  local animator = component.Animator "resources/animations/closed-door.json"
  return graphic, animator
end

return graphics