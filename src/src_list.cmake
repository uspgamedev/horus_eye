SET(GAME_SRCS 
    externals/inifile.cc 
    externals/inifile.h 
    externals/windows/include/dirent.h 
    game/ai.h 
    game/ai/ai.cc 
    game/ai/ai.h 
    game/ai/aidata.cc 
    game/ai/aidata.h 
    game/ai/aimodule.h 
    game/ai/blocks/followtarget.cc 
    game/ai/blocks/followtarget.h 
    game/ai/blocks/randommovement.cc 
    game/ai/blocks/randommovement.h 
    game/ai/blocks/targetdetector.cc 
    game/ai/blocks/targetdetector.h 
    game/ai/blocks/useweapon.cc 
    game/ai/blocks/useweapon.h 
    game/ai/conditionmodule.cc 
    game/ai/conditionmodule.h 
    game/ai/delaymodule.cc 
    game/ai/delaymodule.h 
    game/ai/logicblock.h 
    game/ai/logicmodule.cc 
    game/ai/logicmodule.h 
    game/ai/randommodule.cc 
    game/ai/randommodule.h 
    game/ai/sequencemodule.cc 
    game/ai/sequencemodule.h 
    game/builders/aibuilder.cc 
    game/builders/aibuilder.h 
    game/builders/builder.cc 
    game/builders/builder.h 
    game/builders/collision.cc 
    game/builders/collision.h 
    game/builders/doodadbuilder.cc 
    game/builders/doodadbuilder.h 
    game/builders/explosionbuilder.cc 
    game/builders/explosionbuilder.h 
    game/builders/functions/carrier.cc 
    game/builders/functions/carrier.h 
    game/builders/goodmenubuilder.cc 
    game/builders/goodmenubuilder.h 
    game/builders/herobuilder.cc 
    game/builders/herobuilder.h 
    game/builders/itembuilder.cc 
    game/builders/itembuilder.h 
    game/builders/mummybuilder.cc 
    game/builders/mummybuilder.h 
    game/builders/projectilebuilder.cc 
    game/builders/projectilebuilder.h 
    game/builders/scriptbuilder.cc 
    game/builders/scriptbuilder.h 
    game/campaigns.h 
    game/campaigns/campaign.cc 
    game/campaigns/campaign.h 
    game/campaigns/campaigndescriptor.cc 
    game/campaigns/campaigndescriptor.h 
    game/components.h 
    game/components/animation.cc 
    game/components/animation.h 
    game/components/animator.cc 
    game/components/animator.h 
    game/components/base.h 
    game/components/body.cc 
    game/components/body.h 
    game/components/caster.cc 
    game/components/caster.h 
    game/components/condition.cc 
    game/components/condition.h 
    game/components/controller.h 
    game/components/damageable.cc 
    game/components/damageable.h 
    game/components/direction.cc 
    game/components/direction.h 
    game/components/graphic.cc 
    game/components/graphic.h 
    game/components/lightemitter.cc 
    game/components/lightemitter.h 
    game/components/logic/follower.cc 
    game/components/logic/follower.h 
    game/components/orders.h 
    game/components/playercontroller.cc 
    game/components/playercontroller.h 
    game/components/statecontroller.h 
    game/components/walker.cc 
    game/components/walker.h 
    game/constants.cc 
    game/constants.h 
    game/context.cc 
    game/context.h 
    game/contextloader.cc 
    game/contextscene.cc 
    game/core/coordinates.cc 
    game/core/coordinates.h 
    game/initializer.cc 
    game/initializer.h 
    game/main.cc 
    game/map.h 
    game/map/giantfloor.cc 
    game/map/giantfloor.h 
    game/map/loader.cc 
    game/map/loader.h 
    game/map/room.cc 
    game/map/room.h 
    game/map/specialwall.cc 
    game/map/specialwall.h 
    game/map/tile.cc 
    game/map/tile.h 
    game/renders/profiler.cc 
    game/renders/profiler.h 
    game/renders/shape.cc 
    game/renders/shape.h 
    game/resources/capacityblocks.h 
    game/resources/containedresource.h 
    game/resources/countableresource.h 
    game/resources/energy.h 
    game/resources/resource.h 
    game/scenes.h 
    game/scenes/console.cc 
    game/scenes/console.h 
    game/scenes/gamelayer.h 
    game/scenes/imagescene.cc 
    game/scenes/imagescene.h 
    game/scenes/scrollingimagescene.cc 
    game/scenes/scrollingimagescene.h 
    game/scenes/world.cc 
    game/scenes/world.h 
    game/skills.h 
    game/skills/combatart.cc 
    game/skills/combatart.h 
    game/skills/divinegift.cc 
    game/skills/divinegift.h 
    game/skills/hero/herofireball.cc 
    game/skills/hero/herolight.cc 
    game/skills/hero/herolightning.cc 
    game/skills/hero/heromagicmissile.cc 
    game/skills/initskills.cc 
    game/skills/initskills.h 
    game/skills/mummy/mummyskills.cc 
    game/skills/mummy/pharaohskills.cc 
    game/skills/skill.h 
    game/skills/usearguments.h 
    game/sprites.h 
    game/sprites/effect.h 
    game/sprites/objecthandle.cc 
    game/sprites/objecthandle.h 
    game/sprites/worldobject.cc 
    game/sprites/worldobject.h 
    game/utils/hud.cc 
    game/utils/hud.h 
    game/utils/hudimagefactory.cc 
    game/utils/hudimagefactory.h 
    game/utils/isometricanimationset.cc 
    game/utils/isometricanimationset.h 
    game/utils/levelloader.cc 
    game/utils/levelloader.h 
    game/utils/menuimagefactory.cc 
    game/utils/menuimagefactory.h 
    game/utils/settings.cc 
    game/utils/settings.h 
    game/utils/visionstrategy.cc 
    game/utils/visionstrategy.h 
)
