from ugdk.ugdk_drawable import Sprite
from ugdk.ugdk_base import ResourceManager_GetSpritesheetFromTag as GetSpritesheetFromTag, ResourceManager_GetAnimationSetFromFile as GetAnimationSetFromFile
from ugdk.pyramidworks_geometry import *

def generate():
    d = {}
    d['drawable'] = Sprite(GetSpritesheetFromTag("explosion"), GetAnimationSetFromFile("animations/explosion.gdd"))
    d['drawable'].SelectAnimation("HERO_FIREBALL_WEAPON")
    
    d['collision'] = {
        "class": "Button",
        "shape": Rect(1.0, 1.0),
        "known_collision": [ ["Hero", "deal_damage", 100.0 ] ]
    }
    return d