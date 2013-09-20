
require 'ugdk.math'
require 'component'
require 'ai'

local vec = ugdk_math.Vector2D

local function Spawn (recipe, points)
  local logic = proxy 'LogicBlock'
  function logic:Start ()
    print "MWHAHAHAHAHA"
  end
  function logic:Update (dt, data)
    print "D'OH"
  end
  function logic:End ()
    print "BUH-BYE"
  end
end

function generate (oAI, recipe, delay, ...)
  -- Decrypt arguments >.<
  print(recipe, delay)
  for _,v in ipairs{...} do
    local p
    for x,y in string.gmatch(v, "(%d+%.?%d*) +(%d+.?%d*)") do
      p = vec(x, y)
    end
    print(p:get_x(), p:get_y())
  end
  -- Setup AI's root
  local delay_root = ai.DelayModule(delay, true)
  oAI:set_root(delay_root)
  delay_root:set_child(LogicModule(Spawn(recipe, points)))
end
