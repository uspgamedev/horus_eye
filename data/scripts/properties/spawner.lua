
require "ugdk.drawable"
require "ugdk.action"
require "ugdk.math"
require "pyramidworks.geometry"
require "component"
require "context"
require "builder"

function build (wobj, params)
  args = builder.StringList();
  args:push_back 'assault'
  args:push_back(params.recipe)
  args:push_back(params.delay)
  for _,v in ipairs(params.points) do
    args:push_back(v:get_x()..' '..v:get_y())
  end
  local spawnerAI = builder.AIScript(args)
  context.AddAIComponent(wobj, spawnerAI)
end
