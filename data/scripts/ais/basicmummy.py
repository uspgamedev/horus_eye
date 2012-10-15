import ugdk_action #POG
from pyramidworks_geometry import Circle
from component import WorldObject, Controller
from ai import SequenceModule, LogicModule, FollowTarget, RandomMovement, TargetDetector, UseWeapon

#Always remember: we should create the AIs (and their AIModule trees) in a top-down method...
#                 like in the following order:
#	AI -> root AIModule -> AI.SetRootModule -> [ other module -> set module as child from parent ]* -> return AI 


def generate(oAI, *args):
    print "Generating Basic Mummy AI tree"
    print "for AI %s, owner = %s|%s" % (oAI.name(), oAI.owner().identifier(), oAI.owner().tag())
    ####
    mainList = SequenceModule()
    oAI.set_root(mainList)
    ####
    detectHeroLogic = TargetDetector()
    detectHeroLogic.set_target_classname("Hero")
    detectHeroLogic.set_area( Circle(10.0) )
    detectHeroLogic.set_identifier("hero")
    detectHero = LogicModule( detectHeroLogic )
    mainList.AddChildModule(detectHero)
    ####
    randomMoveLogic = RandomMovement(1.0)
    randomMove = LogicModule( randomMoveLogic )
    mainList.AddChildModule(randomMove)
    ####
    followHeroLogic = FollowTarget()
    followHeroLogic.set_detector_identifier("hero")
    followHero = LogicModule( followHeroLogic )
    detectHero.set_child(followHero)
    ####
    attackLogic = UseWeapon(Controller.PRIMARY)
    attack = LogicModule( attackLogic )
    followHero.set_child(attack)
    

