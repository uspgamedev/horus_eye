
require "component"
require "map"
require "constants"
require "ugdk.math"
require "pyramidworks.geometry"
require "event"

local roomsize = 12
local vec2 = ugdk_math.Vector2D

math.randomseed(os.time())
math.random()
math.random()

music = "musics/Arabesque.ogg"
rooms = {
  {0, 0, "entrance"},
  {11, 2, "divergence"},
  {2, 12, "spike_room"},
  {22, 3, "fireball_room"},
  {8, 18, "central_room"},
  {0, 21, "explo_room"},
  {5, 35, "assault_room"},
  {25, 20, "spawner_room"}
}

width = 100
height = 100

function new()
  local level = {}
  
  function level:Start(native, campaign)
    local hero = builder.Kha()
    hero:AddDeathEvent(campaign:create_hero_death_callback())
    native:SetHero(hero)
    
    local initial_room = native:GetRoom "entrance"
    initial_room:AddObject(hero, ugdk_math.Vector2D(2, roomsize / 2), true)
    native:ChangeFocusedRoom(initial_room)
    
    -- Don't hold these shared_ptr in scripts...
    hero = nil
    collectgarbage()
  end
  
  function level:End(native, campaign)
  end
  
  return level
end