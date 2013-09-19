
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
  local spawnerAI = builder.AIScript(wobj, args)
  context.AddAIComponent(wobj, spawnerAI)
end
