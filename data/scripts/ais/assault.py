from math import pi, log
from random import random
import ugdk_action #POG
from ugdk_math import Vector2D
from pyramidworks_geometry import Circle
from component import WorldObject, Controller
from ai import AIModule, SequenceModule, LogicModule, LogicBlock

def generate(oAI, *args):
    ####
    mainList = SequenceModule()
    oAI.set_root(mainList)

