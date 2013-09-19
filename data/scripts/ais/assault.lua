
require 'ugdk.math'
require 'component'
require 'ai'

local vec = ugdk_math.Vector2D

function generate (oAI, recipe, ...)
  local mainList = ai.SequenceModule()
  oAI:set_root(mainList)
  print(recipe)
  for x,y in string.gmatch(..., "(%d+%.?%d*) +(%d+.?%d*)") do
    p = vec(x, y)
  end
  print(p:get_x(), p:get_y())
end
