
require "component"
require "map"
require "constants"
require "ugdk.math"
require "pyramidworks.geometry"
require "event"

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
  {22, 3, "fireball_room"},
  --{roomsize * 3, 2, "firstblood"},
  --{roomsize * 4, 5, "exit"}
}

start_position = {"hero_room", 2, roomsize / 2}

width = 100
height = 100

event.Clear "LOAD_ENTRANCE"
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

event.Clear "DOOR-COUNTER"
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
        end
      end
    )
    self:MakeRecipe("door", ugdk_math.Vector2D(10, 5), DOOR(1))
    self:MakeRecipe("door", ugdk_math.Vector2D(10, 6), DOOR(2))
    self:MakeRecipe("door-switch", ugdk_math.Vector2D(9, 2))
    self:MakeRecipe("door-switch", ugdk_math.Vector2D(9, 9))

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
    --TODO: Modularizar o teco abaixo se a gente for usar mais
    --["spike-exit-event"] = {
    --  property = "event_region",
    --  params = {
    --    shape = pyramidworks_geometry.Rect(1.0, 2.0),
    --    callback = function (region)
    --      local room = region:current_room()
    --      context.ActivateRoom "spike_room"
    --      room:WorldObjectByTag("SPIKEDOOR-1"):Die()
    --      room:WorldObjectByTag("SPIKEDOOR-2"):Die()
    --    end
    --  }
    --},
    --["fireball-exit-event"] = {
    --  property = "event_region",
    --  params = {
    --    shape = pyramidworks_geometry.Rect(1.0, 2.0),
    --    callback = function (region)
    --      local room = region:current_room()
    --      context.ActivateRoom "fireball_room"
    --      room:WorldObjectByTag("FIREDOOR-1"):Die()
    --      room:WorldObjectByTag("FIREDOOR-2"):Die()
    --    end
    --  }
    --},
    ["urn"] = { property = "urn" },
    ["spike-door"] = {
      property = "open-door",
      params = {
        dir = "LEFT",
        open_event = function () context.ActivateRoom "spike_room" end
      }
    },
    ["fireball-door"] = {
      property = "open-door",
      params = {
        dir = "LEFT",
        open_event = function () context.ActivateRoom "fireball_room" end
      }
    },
    ["closed-door"] = { property = "closed-door", params = { dir = "Right" } },
    ["eventswitch"] = { property = "event_switch", params = { "DOOR-COUNTER" } }
  },
  collision_classes = {
    { "Switch", "Wall" },
    { "EventArea" }
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
    self:MakeRecipe("fireball-door", ugdk_math.Vector2D(10, 3), "FIREDOOR-1")
    self:MakeRecipe("fireball-door", ugdk_math.Vector2D(10, 4), "FIREDOOR-2")
    self:MakeRecipe("spike-door", ugdk_math.Vector2D(0, 12), "SPIKEDOOR-1")
    self:MakeRecipe("spike-door", ugdk_math.Vector2D(0, 13), "SPIKEDOOR-2")
    self:MakeRecipe("closed-door", ugdk_math.Vector2D(4, 15), "NEXTDOOR-1")
    self:MakeRecipe("closed-door", ugdk_math.Vector2D(5, 15), "NEXTDOOR-2")
    self:MakeRecipe("closed-door", ugdk_math.Vector2D(6, 15), "NEXTDOOR-3")
  end
}

event.Clear "FALL_SPIKES"
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
    ["fall-spikes-trigger"] = { property = "trigger", params = { activates = "FALL_SPIKES", delay = 0.4 } },
    ["spike-trap"] = { property = "spike-trap" },
    ["spike-trap-event"] = {
      property = "event_region",
      params = {
        shape = pyramidworks_geometry.Rect(1.0, 4.0),
        callback = function (region)
          local room = region:current_room()
          event.Register(
            "FALL_SPIKES",
            function ()
              room:MakeRecipe("spike-trap", ugdk_math.Vector2D(5, 1))
              room:MakeRecipe("spike-trap", ugdk_math.Vector2D(5, 2))
              room:MakeRecipe("spike-trap", ugdk_math.Vector2D(5, 3))
              room:MakeRecipe("spike-trap", ugdk_math.Vector2D(5, 4))
              event.Clear "FALL_SPIKES"
            end
          )
          room:MakeRecipe "fall-spikes-trigger"
        end
      }
    },
    ["urn"] = { property = "urn" },
    ["door-left"] = { property = "closed-door", params = { dir = "Left" } },
    ["door-right"] = { property = "closed-door", params = { dir = "Right" } },
    ["eventbutton"] = { property = "event_button", params = { "DIVERGENCE-DOOR-COUNTER" } }
  },
  collision_classes = {
    {"Spike"},
    {"EventArea"}
  },
  setup = function(self)
    self:MakeRecipe("eventbutton", ugdk_math.Vector2D(2,2.5))
    self:MakeRecipe("spike-trap-event", ugdk_math.Vector2D(5,2.5))
  end
}

fireball_room = {
  width = 9,
  height = 6,
  matrix = [[
%%%%%%%%%
%.......%
........%
........%
#.......%
####&####
]],
  recipes = {
    ["fireball-trap"] = { property = "fireball-trap", params = {dir = ugdk_math.Vector2D(0, -1)}},
    ["fireball-event"] = {
      property = "event_region",
      params = {
        shape = pyramidworks_geometry.Rect(1.0, 4.0),
        callback = function (region)
          local room = region:current_room()
          room:MakeRecipe("fireball-trap", ugdk_math.Vector2D(4, 5))
        end
      }
    },
    ["eventbutton"] = { property = "event_button", params = { "DIVERGENCE-DOOR-COUNTER" } }
  },
  collision_classes = {
    { "EventArea" }
  },
  setup = function(self)
    self:MakeRecipe("eventbutton", ugdk_math.Vector2D(4,2.5))
    self:MakeRecipe("fireball-event", ugdk_math.Vector2D(4,2.5))
  end
}