from math import pi
import ugdk_action #POG
from ugdk_math import Vector2D
from pyramidworks_geometry import Circle
from component import WorldObject, Controller
from ai import AIModule, SequenceModule, LogicModule, FollowTarget, RandomMovement, TargetDetector, UseWeapon, LogicBlock

#Always remember: we should create the AIs (and their AIModule trees) in a top-down method...
#                 like in the following order:
#	AI -> root AIModule -> AI.SetRootModule -> [ other module -> set module as child from parent ]* -> return AI 


class Evasion(LogicBlock):
    def __init__(self, detector_identifier):
        self.detector_identifier = detector_identifier
        #since there is no way for us to get the direction of the Projectile (WObj), we POG!
        self.got_ltp = False
        self.last_target_pos = Vector2D()
        self.target_move = Vector2D()
        
    def Start(self):
        pass
        
    def Update(self, dt, data):
        owner = self.parent().base().owner()
        if owner.dead():   return AIModule.DORMANT
        
        target = data.GetSharedData(self.detector_identifier + "_target")
        if target == None:
            #print "No target to evade from"
            self.got_ltp = False
            return AIModule.ACTIVE
            
        if not self.got_ltp:
            self.last_target_pos.set_x(target.world_position().get_x())
            self.last_target_pos.set_y(target.world_position().get_y())
            self.got_ltp = True
            #print "pogging"
            return AIModule.ACTIVE
            
        self.target_move = (target.world_position() - self.last_target_pos).Normalize()
        self.last_target_pos.set_x(target.world_position().get_x())
        self.last_target_pos.set_y(target.world_position().get_y())
        
        target_dir = (owner.world_position() - target.world_position()).Normalize()
        #print "TargetMove = (%s, %s) :: TargetDir = (%s, %s)  |:| dot = %s" % (self.target_move.get_x(), self.target_move.get_y(), target_dir.get_x(), target_dir.get_y(), self.target_move * target_dir)
        if self.target_move * target_dir < 0.5:
            #print "projectile not coming our way"
            return AIModule.ACTIVE

        tdPerp = Vector2D(-target_dir.get_y(), target_dir.get_x())
        side = 1.0 - 2*( tdPerp * self.target_move > 0 )
        angle = side * pi / 2
        target_dir = target_dir.Rotate(angle)
        
        #print "evading"
        data.set_direction(target_dir)
        return AIModule.ACTIVE

    def End(self):
        pass
        

def generate(oAI, *args):
    ####
    mainList = SequenceModule()
    oAI.set_root(mainList)
    
    def createDetectHeroModule():
        detectHeroLogic = TargetDetector()
        detectHeroLogic.set_target_classname("Hero")
        detectHeroLogic.set_area( Circle(10.0) )
        detectHeroLogic.set_identifier("hero")
        return LogicModule( detectHeroLogic )
        
    
    ####
    detectHero = createDetectHeroModule()
    mainList.AddChildModule(detectHero)
    ####
    mainList.AddChildModule(LogicModule(RandomMovement(1.0)))
    ####
    followHeroLogic = FollowTarget()
    followHeroLogic.set_detector_identifier("hero")
    followHero = LogicModule( followHeroLogic )
    detectHero.set_child(followHero)
    ####
    attackLogic = UseWeapon(Controller.PRIMARY)
    attack = LogicModule( attackLogic )
    followHero.set_child(attack)
    
    ####
    #detectShotLogic = TargetDetector()
    #detectShotLogic.set_target_classname("Projectile")
    #detectShotLogic.set_area( Circle(10.0) )
    #detectShotLogic.set_identifier("shot")
    #detectShot = LogicModule( detectShotLogic )
    #attack.set_child(detectShot)
    ##
    #evadeLogic = Evasion("shot")
    #evade = LogicModule( evadeLogic )
    #detectShot.set_child( evade )
    

