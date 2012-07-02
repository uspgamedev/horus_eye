from ugdk.ugdk_drawable import Sprite
from ugdk.ugdk_base import ResourceManager_GetSpritesheetFromTag as GetSpritesheetFromTag, ResourceManager_GetAnimationSetFromFile as GetAnimationSetFromFile
from ugdk.pyramidworks_collision import *
from ugdk.pyramidworks_geometry import *

import random

names = ["images/explosion.png", "images/pharaoh_120x140.png", "images/shield.png" ]

def on_die_callback():
    print "OH NOES MORRI"

def generate():
    d = {}
    random.shuffle(names)
    d['drawable'] = Sprite(GetSpritesheetFromTag(names[0]), GetAnimationSetFromFile("animations/explosion.gdd"))
    d['drawable'].SelectAnimation("HERO_FIREBALL_WEAPON")
    d['timed_life'] = 5.0
    d['on_die_callback'] = on_die_callback

    d['collision_object'] = CollisionObject()
    return d