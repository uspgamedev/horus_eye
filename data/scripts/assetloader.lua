
require 'ugdk.graphic'
require 'ugdk.resource'
require 'ugdk.math'

assetloader = {}

local atlas_list = {
  items           = "resources/images/items-sheet",
  projectile      = "resources/images/projectile-sheet",
  scenery         = "resources/images/scenery-sheet",
  switch          = "resources/images/switch-sheet",
  wall            = "resources/images/wall-sheet",
  hero            = "resources/images/character/hero-sheet",
  ["mummy-blue"]  = "resources/images/character/mummy-blue-sheet",
  ["mummy-green"] = "resources/images/character/mummy-green-sheet",
  ["mummy-red"]   = "resources/images/character/mummy-red-sheet",
  pharaoh         = "resources/images/character/pharaoh-sheet",
}

function assetloader.LoadAssets()
  for tag, file in pairs(atlas_list) do
    context.LoadTextureAtlas(tag, file)
  end
end