
require 'ugdk.math'
require 'component'
require 'ai'
require 'context'

local vec = ugdk_math.Vector2D

local function Spawn (recipe, points)
  local logic = proxy 'LogicBlock'
  function logic:Start ()
    -- nothing
  end
  function logic:Update (dt, data)
    local room = context.hero():current_room()
    for _,point in ipairs(points) do
      room:MakeRecipe(recipe, point)
    end
    return ai.AIModule_ACTIVE
  end
  function logic:End ()
    -- nothing
  end
  return logic
end

function generate (oAI, recipe, delay, ...)
  -- Decrypt arguments >.<
  local points = {}
  for _,v in ipairs{...} do
    local p
    for x,y in string.gmatch(v, "(%d+%.?%d*) +(%d+.?%d*)") do
      p = vec(x, y)
    end
    table.insert(points, p)
  end
  -- Setup AI's root
  local delay_root = ai.DelayModule(delay, true)
  oAI:set_root(delay_root)
  delay_root:set_child(ai.LogicModule(Spawn(recipe, points)))
end
