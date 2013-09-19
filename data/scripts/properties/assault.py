
import ugdk_math
import context
import builder

def build (wobj, params):
  spawnerAI = builder.AIScript(wobj, "assault")
  context.AddAIComponent(wobj, spawnerAI)
