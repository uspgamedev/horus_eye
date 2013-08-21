
require "component"
require "map"
require "constants"
require "ugdk.math"
require "pyramidworks.geometry"
require "event"

local roomsize = 12
local vec2 = ugdk_math.Vector2D

music = "musics/Arabesque.ogg"
rooms = {
  {0, 0, "entrance"},
  {11, 2, "divergence"},
  {2, 12, "spike_room"},
  {22, 3, "fireball_room"},
  {8, 18, "central_room"},
  {0, 21, "explo_room"}
}

start_position = {"entrance", 2, roomsize / 2}

width = 100
height = 100

fireball_room = {
  neighborhood = { "divergence" },
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
    ["fireball-trap"] = {
      property = "fireball-trap",
      params = {dir = vec2(0, -1)}
    },
    ["fireball-event"] = {
      property = "event_region",
      params = {
        shape = pyramidworks_geometry.Rect(1.0, 4.0),
        callback = function (region)
          local room = region:current_room()
          room:MakeRecipe("fireball-trap", vec2(4, 5))
        end
      }
    },
    ["eventbutton"] = {
      property = "event_button",
      params = { "DIVERGENCE-DOOR-COUNTER" }
    }
  },
  collision_classes = {
    { "EventArea" }
  },
  setup = function(self)
    self:MakeRecipe("eventbutton", vec2(4,2.5))
    self:MakeRecipe("fireball-event", vec2(4,2.5))
  end
}

local function dual_spawner(offset)
  return {
    property = "spawn_region",
    params = {
      shapefactory = function ()
        return pyramidworks_geometry.Rect(2.0, 2.0)
      end,
      id = 64,
      tag = "Mummy",
      recipe = "mummy",
      multiple = {
        offset,
        -offset,
      }
    }
  }
end

central_room = {
  neighborhood = {
    "divergence", "explo_room"
  },
  width = 17,
  height = 17,
  matrix = [[
%%%%%%%...%%%%%%%
%%%%%%%...%%%%%%%
%%%%.........%%%%
%%%%.........%%%%
%%.............%%
%%....%...%....%%
%%...%%...%%...%%
.................
.................
......M...M......
%%...%%...%%...##
%%....%...%....##
%%.............##
%%%%.........####
%%%%..M...M..####
%%%%%%%...#######
%%%%%%%...#######
]],
  recipes = {
    ["horizontal-mummy-spawn"] = dual_spawner(vec2(4, 0)),
    ["vertical-mummy-spawn"] = dual_spawner(vec2(0, 4)),
    ["mummy"] = {
      property = "event_mummy",
      params = {
        spritesheet = "mummy_basic",
        life = constants.GetInt "MUMMY_LIFE",
        radius = constants.GetDouble "MUMMY_RADIUS",
        speed = constants.GetDouble "MUMMY_SPEED"
      }
    },
    ["open-door"] = {
      property = "open-door",
      params = {
        dir = "LEFT",
      }
    },
  },
  setup = function (self)
    self:MakeRecipe("horizontal-mummy-spawn", vec2(8, 5.5))
    self:MakeRecipe("horizontal-mummy-spawn", vec2(8, 16-5.5))
    self:MakeRecipe("vertical-mummy-spawn", vec2(5.5, 8))
    self:MakeRecipe("vertical-mummy-spawn", vec2(16-5.5, 8))
    self:MakeRecipe("open-door", vec2(0, 7))
    self:MakeRecipe("open-door", vec2(0, 8))
    self:MakeRecipe("open-door", vec2(0, 9))
  end
}

explo_room = {
  neighborhood = { "central_room" },
  width = 8,
  height = 11,
  matrix = [[
%%%%%%%%
%......%
%.M..M.%
%......%
%.......
%.......
%.......
%......#
%.M..M.#
%......#
########
]],
  recipes = {
    ["urn"] = { property = "urn", params = { drops = {"FireballExplosion"} } },
  },
  setup = function (self) 
    for i=1,6 do  
      self:MakeRecipe("urn", vec2(0.75+(i-1), 2.75))
      self:MakeRecipe("urn", vec2(1.25+(i-1), 3.25))
      self:MakeRecipe("urn", vec2(0.75+(i-1), 6.75))
      self:MakeRecipe("urn", vec2(1.25+(i-1), 7.25))
    end
  end
}

