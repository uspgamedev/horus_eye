from ugdk.ugdk_drawable import Sprite
from ugdk.ugdk_base import ResourceManager_GetSpritesheetFromTag as GetSpritesheetFromTag

import random

names = ["images/explosion.png", "images/pharaoh_120x140.png", "images/shield.png" ]

def generate():
    d = {}
    random.shuffle(names)
    d['drawable'] = Sprite(GetSpritesheetFromTag(names[0]))
    d['timed_life'] = 5.0
    return d