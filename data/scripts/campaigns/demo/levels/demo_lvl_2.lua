
require "component"
require "map"
require "constants"
require "ugdk.math"
require "pyramidworks.geometry"
require "data.scripts.event"

local roomsize = 12
local function DOOR (i)
  return "THE-DOOR-"..i
end

music = "musics/Arabesque.ogg"
rooms = {
  {0, 0, "hero_room"},
  {0, 0, "entrance"},
  {11, 2, "divergence"},
  {2, 12, "spike_room"},
  --{roomsize * 3, 2, "firstblood"},
  --{roomsize * 4, 5, "exit"}
}

start_position = {"hero_room", 2, roomsize / 2}

width = 100
height = 100

hero_room = {
  width = 1,
  height = 1,
  matrix = [[.]],
  objects = {},
  recipes = {
    ["load-entrance"] = { property = "trigger", params = { activates = "LOAD_ENTRANCE", delay = 0.0 } },
  },
  collision_classes = {},
  setup = function(self) 
    self:MakeRecipe "load-entrance"
    event.Register(
      "LOAD_ENTRANCE",
      function ()
        context.ActivateRoom "entrance"
        event.Clear "LOAD_ENTRANCE"
      end
    )
  end
}

entrance = {
  width = 11,
  height = 12,
  matrix = [[
%%%%%%%%%%%
%%%.......%
%%%.......%
%.........%
%.......%%%
%..........
%..........
%.......%%%
%.........%
%%%.......%
%%%.......%
%%%%%%%%%%%
]],
  objects = {},
  recipes = {
    ["urn"] = { property = "urn" },
    ["door"] = { property = "closed-door", params = { dir = "Left" } },
    ["door-switch"] = { property = "event_switch", params = { "DOOR-COUNTER" } },
    --["exit-event"] = {
    --  property = "event_region",
    --  params = {
    --    shape = pyramidworks_geometry.Rect(1.0, 2.0),
    --    callback = function (region)
    --      local room = region:current_room()
    --      context.ActivateRoom "divergence"
    --    end
    --  }
    --}
  },
  collision_classes = {
    { "Switch", "Wall" }
  },
  setup = function(self)
    local switch_counter = 2
    event.Register(
      "DOOR-COUNTER",
      function ()
        switch_counter = switch_counter - 1
        if switch_counter <= 0 then
          for i=1,2 do
            self:WorldObjectByTag(DOOR(i)):Die()
          end
          event.Clear "DOOR-COUNTER"
          context.ActivateRoom "divergence"
          --context.ActivateRoom "spike_room"
        end
      end
    )
    self:MakeRecipe("door", ugdk_math.Vector2D(10, 5), DOOR(1))
    self:MakeRecipe("door", ugdk_math.Vector2D(10, 6), DOOR(2))
    self:MakeRecipe("door-switch", ugdk_math.Vector2D(9, 2))
    self:MakeRecipe("door-switch", ugdk_math.Vector2D(9, 9))
    --self:MakeRecipe("exit-event", ugdk_math.Vector2D(9, roomsize/2))

    for i = 1,2 do
      local x,y = 4.5, 1.2+8.6*(i-1)
      for i = 1,6 do
        self:MakeRecipe("urn", ugdk_math.Vector2D(x+0.6*(i-1), y))
      end
    end
  end
}

divergence = {
  width = 11,
  height = 16,
  matrix = [[
%%%%%%%%%%%
%%%..%..%%%
%%.......%%
...........
...........
%%.......##
%.........#
%.........#
%%.......##
%.........#
%.........#
%%.......##
..........#
..........#
##.......##
####...####
]],
  objects = {},
  recipes = {
    --["load_corridor_trigger"] = { property = "trigger", params = { activates = "LOAD_CORRIDOR", delay = 0.0 } },
    ["spike-exit-event"] = {
      property = "event_region",
      params = {
        shape = pyramidworks_geometry.Rect(1.0, 2.0),
        callback = function (region)
          local room = region:current_room()
          context.ActivateRoom "spike_room"
          room:WorldObjectByTag("SPIKEDOOR-1"):Die()
          room:WorldObjectByTag("SPIKEDOOR-2"):Die()
        end
      }
    },
    ["urn"] = { property = "urn" },
    ["door-left"] = { property = "closed-door", params = { dir = "Left" } },
    ["door-right"] = { property = "closed-door", params = { dir = "Right" } },
    ["eventswitch"] = { property = "event_switch", params = { "DOOR-COUNTER" } }
  },
  collision_classes = {
    { "Switch", "Wall" },
    {"EventArea"}
  },
  setup = function(self)
    self:MakeRecipe("urn", ugdk_math.Vector2D(3, 1))
    self:MakeRecipe("urn", ugdk_math.Vector2D(3.5, 1))
    self:MakeRecipe("urn", ugdk_math.Vector2D(4, 1))
    self:MakeRecipe("urn", ugdk_math.Vector2D(6, 1))
    self:MakeRecipe("urn", ugdk_math.Vector2D(6.5, 1))
    self:MakeRecipe("urn", ugdk_math.Vector2D(7, 1))
    self:MakeRecipe("urn", ugdk_math.Vector2D(1, 6))
    self:MakeRecipe("urn", ugdk_math.Vector2D(1, 6.5))
    self:MakeRecipe("urn", ugdk_math.Vector2D(1, 7))
    self:MakeRecipe("urn", ugdk_math.Vector2D(1, 9))
    self:MakeRecipe("urn", ugdk_math.Vector2D(1, 9.5))
    self:MakeRecipe("urn", ugdk_math.Vector2D(1, 10))
    self:MakeRecipe("urn", ugdk_math.Vector2D(9, 6))
    self:MakeRecipe("urn", ugdk_math.Vector2D(9, 6.5))
    self:MakeRecipe("urn", ugdk_math.Vector2D(9, 7))
    self:MakeRecipe("urn", ugdk_math.Vector2D(9, 9))
    self:MakeRecipe("urn", ugdk_math.Vector2D(9, 9.5))
    self:MakeRecipe("urn", ugdk_math.Vector2D(9, 10))
    self:MakeRecipe("urn", ugdk_math.Vector2D(9, 12))
    self:MakeRecipe("urn", ugdk_math.Vector2D(9, 12.5))
    self:MakeRecipe("urn", ugdk_math.Vector2D(9, 13))
    self:MakeRecipe("door-left", ugdk_math.Vector2D(10, 3), "FIREDOOR-1")
    self:MakeRecipe("door-left", ugdk_math.Vector2D(10, 4), "FIREDOOR-2")
    self:MakeRecipe("door-left", ugdk_math.Vector2D(0, 12), "SPIKEDOOR-1")
    self:MakeRecipe("door-left", ugdk_math.Vector2D(0, 13), "SPIKEDOOR-2")
    self:MakeRecipe("door-right", ugdk_math.Vector2D(4, 15), "NEXTDOOR-1")
    self:MakeRecipe("door-right", ugdk_math.Vector2D(5, 15), "NEXTDOOR-2")
    self:MakeRecipe("door-right", ugdk_math.Vector2D(6, 15), "NEXTDOOR-3")
    self:MakeRecipe("spike-exit-event", ugdk_math.Vector2D(2, 12))
  end
}

spike_room = {
  width = 9,
  height = 6,
  matrix = [[
%%%%%%%%%
%.......%
%........
%........
%.......#
#########
]],
  recipes = {
    --["load_corridor_trigger"] = { property = "trigger", params = { activates = "LOAD_CORRIDOR", delay = 0.0 } },
    ["urn"] = { property = "urn" },
    ["door-left"] = { property = "closed-door", params = { dir = "Left" } },
    ["door-right"] = { property = "closed-door", params = { dir = "Right" } },
    ["eventbutton"] = { property = "event_button", params = { "DIVERGENCE-DOOR-COUNTER" } }
  },
  setup = function(self)
    self:MakeRecipe("eventbutton", ugdk_math.Vector2D(2,2.5))
  end
}

