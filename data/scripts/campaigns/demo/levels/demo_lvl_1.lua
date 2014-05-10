require "component"
require "map"
require "constants"
require "ugdk.math"
require "pyramidworks.geometry"
require "event"

music = "musics/Arabesque.ogg"
roomsize = 16
rooms = {
   {roomsize * 0, 0, "entrance"},
   {roomsize * 1, 5, "opencorridor"},
   {roomsize * 2, 5, "closedcorridor"},
   {roomsize * 3, 2, "firstblood"},
   {roomsize * 4, 5, "exit"}
}

width = roomsize*#rooms
height = roomsize

entrance = {
  neighborhood = { "opencorridor" },
  width = roomsize,
  height = roomsize,
  matrix = [[
%%%%%%%%%%%%%%%%
%..............%
%..............%
%..............%
%..............%
%..%..%..%..%..%
%...............
%...............
%...............
%...............
%..%..%..%..%..#
%..............#
%..............#
%..............#
%..............#
################
]],
  objects = {},
  recipes = {
    ["urn"] = { property = "urn" },
  },
  collision_classes = {
    { "Switch", "Wall" }
  },
  setup = function(self)
    for i = 1,12 do
      local x,y = 1+i, 2
      for i = 1,math.random(3,4) do
        self:MakeRecipe("urn", ugdk_math.Vector2D(x+math.random()*2-1, y+math.random()*2-1))
      end
      y = 13
      for i = 1,math.random(3,4) do
        self:MakeRecipe("urn", ugdk_math.Vector2D(x+math.random()*2-1, y+math.random()*2-1))
      end
    end
  end
}
 
opencorridor = {
  neighborhood = { "entrance", "closedcorridor" },
  width = roomsize,
  height = roomsize - 10,
  matrix = [[
%%%%%%%%%%%%%%%%
...............%
................
................
...............#
#######&########
]],
  objects = {},
  recipes = {
    ["activated_switch"] = { property = "activated_switch" },
    ["dummyspiketrap"] = { property = "dummy-spike-trap" },
  },
  setup = function(self)
    self:MakeRecipe("activated_switch", ugdk_math.Vector2D(12, 3))
    self:MakeRecipe("dummyspiketrap", ugdk_math.Vector2D(8 + math.random(), 1 + math.random() * 3))
    self:MakeRecipe("dummyspiketrap", ugdk_math.Vector2D(4 + math.random(), 1 + math.random() * 3))
  end

}

closedcorridor = {
  neighborhood = { "opencorridor", "firstblood" },
  width = roomsize,
  height = roomsize - 10,
  matrix = [[
%%%%%%%%%%%%%%%%
...............%
................
................
...............#
#######&########
]],
  objects = {
  },
  collision_classes = {
    { "Switch", "Wall" }
  },
  recipes = {
    ["dummyspiketrap"] = { property = "dummy-spike-trap" },
    ["door"] = { property = "closed-door", params = { dir = "Left" } },
    ["killswitch"] = { property = "kill_switch", params = { "THE-DOOR-1", "THE-DOOR-2" } }
  },
  setup = function(self)
    self:MakeRecipe("dummyspiketrap", ugdk_math.Vector2D(8 + math.random(), 1 + math.random() * 3))
    self:MakeRecipe("dummyspiketrap", ugdk_math.Vector2D(4 + math.random(), 1 + math.random() * 3))
    
    self:MakeRecipe("door", ugdk_math.Vector2D(15, 2), "THE-DOOR-1")
    self:MakeRecipe("door", ugdk_math.Vector2D(15, 3), "THE-DOOR-2")
    
    self:MakeRecipe("killswitch", ugdk_math.Vector2D(12, 2))
  end
}

firstblood = {
  neighborhood = { "closedcorridor", "exit" },
  width = roomsize,
  height = roomsize-4,
  matrix = [[
%%%%%%%%%%%%%%%%
%....%%%%%%%%%%%
%....%%%%%%%%%%%
%....%%%%%%%%%%%
%..............%
................
................
%..............#
%....###########
%....###########
%....###########
################
]],
  objects = {},
  collision_classes = {
    { "EventArea" }
  },
  recipes = {
    ["door"] = { property = "closed-door", params = { dir = "Left" } },
    
    ["delayed_mummy"] = {
      property = "delayed_spawner",
      params = {
        time = 0.5,
        recipe = "surprise_mummy"
      }
    },
    
    ["surprise_mummy"] = {
      property = "trigger_mummy",
      params = {
        spritesheet = "mummy_basic",
        life = constants.GetInt "MUMMY_LIFE",
        radius = constants.GetDouble "MUMMY_RADIUS",
        speed = constants.GetDouble "MUMMY_SPEED",
        triggers = { "THE-DOOR-3", "THE-DOOR-4" }
      }
    },
    ["entrance_event"] = {
      property = "event-region",
      params = {
        shape = pyramidworks_geometry.Rect(1.0, roomsize),
        callback = function (region)
          local room = region:current_room()
          room:MakeRecipe("door", ugdk_math.Vector2D(0, 5), "THE-DOOR-1")
          room:MakeRecipe("door", ugdk_math.Vector2D(0, 6), "THE-DOOR-2")
          room:MakeRecipe("delayed_mummy", ugdk_math.Vector2D(2, 2))
          room:MakeRecipe("delayed_mummy", ugdk_math.Vector2D(2, roomsize-4-2))
        end
      }
    }
  },
  setup = function (room)
    room:MakeRecipe("door", ugdk_math.Vector2D(15, 5), "THE-DOOR-3")
    room:MakeRecipe("door", ugdk_math.Vector2D(15, 6), "THE-DOOR-4")
    room:MakeRecipe("entrance_event", ugdk_math.Vector2D(3, roomsize/2))

    for i=3,4 do
      context.AddDamageableComponent(room, "THE-DOOR-"..i, 2)
    end
  end
}

exit = {
   neighborhood = { "firstblood" },
   width = roomsize,
   height = roomsize - 10,
   matrix = [[
%%%%%%%%%%%%%%%#
...............#
...........D...#
...............#
...............#
################
]],
   objects = {
   },
   setup = function(self)
   end
}

function new()
  local level = {}
  
  function level:Start(native, campaign)
    local hero = builder.Kha()
    hero:AddDeathEvent(campaign:create_hero_death_callback())
    native:SetHero(hero)
    
    local initial_room = native:GetRoom "entrance"
    initial_room:AddObject(hero, ugdk_math.Vector2D(roomsize / 2 - 5, roomsize / 2), true)
    native:ChangeFocusedRoom(initial_room)
    
    -- Don't hold these shared_ptr in scripts...
    hero = nil
    collectgarbage()
  end
  
  function level:End(native, campaign)
  end
  
  return level
end