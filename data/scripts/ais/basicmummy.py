
from component import WorldObject, Controller
from ai import SequenceModule, LogicModule, FollowTarget, RandomMovement, UseWeapon

#Always remember: we should create the AIs (and their AIModule trees) in a top-down method...
#                 like in the following order:
#	AI -> root AIModule -> AI.SetRootModule -> [ other module -> set module as child from parent ]* -> return AI 


def generate(oAI, *args):
    ####
    mainList = SequenceModule()
    oAI.set_root(mainList)
    ####
    followHeroLogic = FollowTarget()
    followHeroLogic.set_target_tag("hero")
    followHero = LogicModule( followHeroLogic )
    mainList.AddChildModule(followHero)
    ####
    randomMoveLogic = RandomMovement(1.0)
    randomMove = LogicModule( randomMoveLogic )
    mainList.AddChildModule(randomMove)
    ####
    attackLogic = UseWeapon(Controller.PRIMARY)
    attack = LogicModule( attackLogic )
    followHero.set_child(attack)
    

