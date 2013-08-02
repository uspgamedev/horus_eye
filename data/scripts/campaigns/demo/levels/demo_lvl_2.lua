
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
  {8, 2, "divergence"},
  --{roomsize * 2, 5, "closedcorridor"},
  --{roomsize * 3, 2, "firstblood"},
  --{roomsize * 4, 5, "exit"}
}

start_position = {"hero_room", 2, roomsize / 2}

width = roomsize*#rooms
height = roomsize

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
  width = roomsize-1,
  height = roomsize,
  matrix = [[
%%%%%%%%%%%
%%%.......%
%%%.......%
%.........%
%.......###
%..........
%..........
%.......###
%.........#
###.......#
###.......#
###########
]],
  objects = {},
  recipes = {
    ["urn"] = { property = "urn" },
    ["door"] = { property = "closed-door", params = { dir = "Left" } },
    ["door-switch"] = { property = "event_switch", params = { "DOOR-COUNTER" } },
    ["exit-event"] = {
      property = "event_region",
      params = {
        shape = pyramidworks_geometry.Rect(1.0, 2.0),
        callback = function (region)
          local room = region:current_room()
          context.ActivateRoom "divergence"
          context.DeactivateRoom "entrance"
        end
      }
    }
  },
  collision_classes = {
    { "EventArea" },
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
        end
      end
    )
    self:MakeRecipe("door", ugdk_math.Vector2D(8, 5), DOOR(1))
    self:MakeRecipe("door", ugdk_math.Vector2D(8, 6), DOOR(2))
    self:MakeRecipe("door-switch", ugdk_math.Vector2D(9, 2))
    self:MakeRecipe("door-switch", ugdk_math.Vector2D(9, 9))
    self:MakeRecipe("exit-event", ugdk_math.Vector2D(9, roomsize/2))

    for i = 1,2 do
      local x,y = 4.5, 1.2+8.6*(i-1)
      for i = 1,6 do
        self:MakeRecipe("urn", ugdk_math.Vector2D(x+0.6*(i-1), y))
      end
    end
  end
}

divergence = {
  width = roomsize-1,
  height = roomsize,
  matrix = [[
...........
...........
...........
...........
...........
...........
...........
...........
...........
...........
...........
...........
]],
  objects = {},
  recipes = {
    --["load_corridor_trigger"] = { property = "trigger", params = { activates = "LOAD_CORRIDOR", delay = 0.0 } },
    ["urn"] = { property = "urn" },
    ["door"] = { property = "closed-door", params = { dir = "Left" } },
    ["eventswitch"] = { property = "event_switch", params = { "DOOR-COUNTER" } }
  },
  collision_classes = {
    { "Switch", "Wall" }
  },
  setup = function(self)
  end
}

