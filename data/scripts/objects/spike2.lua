
require "ugdk.drawable"
require "pyramidworks.geometry"
require "component"
require "context"
require "map"

local Sprite = ugdk_drawable.Sprite
local Rect = pyramidworks_geometry.Rect

function generate ()
  local args = {}
  args.drawable = Sprite("fireball_explosion", "animations/explosion.gdd")
  args.drawable:animation_player():Select("HERO_FIREBALL_WEAPON")

  args.collision = {
    class = "Button",
    shape = Rect(1.0, 1.0),
    known_collision = {
      Hero = function(self, obj) obj:damageable():TakeDamage(200) end
    }
  }

  args.timed_life = 5.0

  args.on_die_callback = function (obj)
    local target = obj:current_room():WorldObjectByTag "EXPLO"
    target:Die()
  end

  return args
end

