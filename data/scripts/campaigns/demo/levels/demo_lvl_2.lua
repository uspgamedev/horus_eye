
require "component"
require "map"
require "constants"
require "ugdk.math"
require "pyramidworks.geometry"
require "data.scripts.event"

local roomsize = 12

music = "musics/Arabesque.ogg"
rooms = {
   {roomsize * 0, 0, "entrance"},
   --{roomsize * 1, 5, "opencorridor"},
   --{roomsize * 2, 5, "closedcorridor"},
   --{roomsize * 3, 2, "firstblood"},
   --{roomsize * 4, 5, "exit"}
}

start_position = {"entrance", roomsize / 2 - 5, roomsize / 2}

width = roomsize*#rooms
height = roomsize

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
    --["load_corridor_trigger"] = { property = "trigger", params = { activates = "LOAD_CORRIDOR", delay = 0.0 } },
    ["urn"] = { property = "urn" },
    ["door"] = { property = "closed-door", params = { dir = "Left" } },
  },
  collision_classes = {
    { "Switch", "Wall" }
  },
  setup = function(self)
    --self:MakeRecipe "load_corridor_trigger"

    --event.Register(
    --  "LOAD_CORRIDOR",
    --  function ()
    --    context.ActivateRoom "opencorridor"
    --    event.Clear "LOAD_CORRIDOR"
    --  end
    --)
    self:MakeRecipe("door", ugdk_math.Vector2D(8, 5), "THE-DOOR-1")
    self:MakeRecipe("door", ugdk_math.Vector2D(8, 6), "THE-DOOR-2")

    for i = 1,2 do
      local x,y = 4.5, 1.2+8.6*(i-1)
      for i = 1,6 do
        self:MakeRecipe("urn", ugdk_math.Vector2D(x+0.6*(i-1), y))
      end
    end
  end
}

