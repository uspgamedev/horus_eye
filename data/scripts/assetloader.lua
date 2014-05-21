
require 'ugdk.graphic'
require 'ugdk.resource'
require 'ugdk.math'

assetloader = {}

local function loadSingleFrameImage(tag, filename)
  local texture = ugdk_resource.GetTextureFromFile(filename)
  local atlas = ugdk_graphic.TextureAtlas(texture, 1)
  atlas:AddPiece("self", ugdk_math.Integer2D(0, 0), atlas:size())
  context.LoadTextureAtlas(tag, atlas)
end

local function loadPlayerSheets()
  local hero_sheet_image = ugdk_resource.GetTextureFromFile "images/sprite-sheet_MOD3.png"
  local hero_atlas = ugdk_graphic.TextureAtlas(hero_sheet_image, 5*9+2)
  local id = 1
  for row=1,9 do
    for col=1,5 do
      hero_atlas:AddPiece(tostring(id), ugdk_math.Integer2D((col-1)*110, (row-1)*110), ugdk_math.Integer2D(110, 110))
      id = id + 1
    end
  end
  hero_atlas:AddPiece(tostring(id), ugdk_math.Integer2D(  0, 10*110), ugdk_math.Integer2D(110, 110))
  hero_atlas:AddPiece(tostring(id+1), ugdk_math.Integer2D(110, 10*110), ugdk_math.Integer2D(110, 110))
  context.LoadTextureAtlas("hero", hero_atlas)
end

function assetloader.LoadAssets()
  context.LoadTextureAtlas("wall", "resources/images/wall-sheet")
  context.LoadTextureAtlas("switch", "resources/images/switch-sheet")
  context.LoadTextureAtlas("items", "resources/images/items-sheet")
  context.LoadTextureAtlas("projectile", "resources/images/projectile-sheet")
  loadSingleFrameImage("dummy-spike-trap", "resources/images/spike-trap/dummy-spike-trap.png")
  loadSingleFrameImage("vase", "images/vase.png")
  loadSingleFrameImage("closed-door", "images/closed-door.png")
  
  loadPlayerSheets()
end