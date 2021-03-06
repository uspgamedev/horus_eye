
require "ugdk.graphic"
require "pyramidworks.geometry"
require "ugdk.math"
require "component"
require "context"

local graphics = require 'graphics'

local Rect = pyramidworks_geometry.Rect

function build (wobj)
  wobj:AddComponent(graphics.dummy_spike_trap())
  return {
    collision = {
      class = "Wall",
      shape = Rect(1,1),
      known_collision = {}
    }
  }
end
