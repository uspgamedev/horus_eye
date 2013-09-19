
require "ugdk.drawable"
require "ugdk.action"
require "ugdk.math"
require "pyramidworks.geometry"
require "component"
require "context"
require "builder"

function build (wobj, params)
  local spawnerAI = builder.AIScript(wobj, 'assault')
  context.AddAIComponent(wobj, spawnerAI)
  return {}
end
