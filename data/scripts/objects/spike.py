
from ugdk.ugdk_drawable import Sprite
from ugdk.pyramidworks_geometry import Rect
from component import WorldObject

def generate():
    d = {}
    d['drawable'] = Sprite("fireball_explosion", "animations/explosion.gdd")
    d['drawable'].animation_player().Select("HERO_FIREBALL_WEAPON")
    
    d['collision'] = {
        "class": "Button",
        "shape": Rect(1.0, 1.0),
        "known_collision": { 
            "Hero": lambda self, obj: self.damageable().TakeDamage(100)
        }
    }
    return d
