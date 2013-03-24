SET(GAME_SRCS 
    ./externals/inifile.cc 
    ./externals/inifile.h 
    ./game/ai.h 
    ./game/ai/ai.cc 
    ./game/ai/ai.h 
    ./game/ai/aidata.cc 
    ./game/ai/aidata.h 
    ./game/ai/aimodule.h 
    ./game/ai/blocks/followtarget.cc 
    ./game/ai/blocks/followtarget.h 
    ./game/ai/blocks/randommovement.cc 
    ./game/ai/blocks/randommovement.h 
    ./game/ai/blocks/targetdetector.cc 
    ./game/ai/blocks/targetdetector.h 
    ./game/ai/blocks/useweapon.cc 
    ./game/ai/blocks/useweapon.h 
    ./game/ai/conditionmodule.cc 
    ./game/ai/conditionmodule.h 
    ./game/ai/logicblock.h 
    ./game/ai/logicmodule.cc 
    ./game/ai/logicmodule.h 
    ./game/ai/randommodule.cc 
    ./game/ai/randommodule.h 
    ./game/ai/sequencemodule.cc 
    ./game/ai/sequencemodule.h 
    ./game/builders/aibuilder.cc 
    ./game/builders/aibuilder.h 
    ./game/builders/builder.cc 
    ./game/builders/builder.h 
    ./game/builders/doodadbuilder.cc 
    ./game/builders/doodadbuilder.h 
    ./game/builders/effectbuilder.cc 
    ./game/builders/effectbuilder.h 
    ./game/builders/entitybuilder.cc 
    ./game/builders/entitybuilder.h 
    ./game/builders/explosionbuilder.cc 
    ./game/builders/explosionbuilder.h 
    ./game/builders/functions/carrier.cc 
    ./game/builders/functions/carrier.h 
    ./game/builders/goodmenubuilder.cc 
    ./game/builders/goodmenubuilder.h 
    ./game/builders/herobuilder.cc 
    ./game/builders/herobuilder.h 
    ./game/builders/itembuilder.cc 
    ./game/builders/itembuilder.h 
    ./game/builders/mummybuilder.cc 
    ./game/builders/mummybuilder.h 
    ./game/builders/projectilebuilder.cc 
    ./game/builders/projectilebuilder.h 
    ./game/builders/scriptbuilder.cc 
    ./game/builders/scriptbuilder.h 
    ./game/builders/taskbuilder.cc 
    ./game/builders/taskbuilder.h 
    ./game/components.h 
    ./game/components/animation.cc 
    ./game/components/animation.h 
    ./game/components/base.h 
    ./game/components/basegraphic.cc 
    ./game/components/basegraphic.h 
    ./game/components/caster.cc 
    ./game/components/caster.h 
    ./game/components/condition.cc 
    ./game/components/condition.h 
    ./game/components/controller.h 
    ./game/components/damageable.cc 
    ./game/components/damageable.h 
    ./game/components/direction.cc 
    ./game/components/direction.h 
    ./game/components/graphic.cc 
    ./game/components/graphic.h 
    ./game/components/logic/follower.cc 
    ./game/components/logic/follower.h 
    ./game/components/orders.h 
    ./game/components/playercontroller.cc 
    ./game/components/playercontroller.h 
    ./game/components/shape.cc 
    ./game/components/shape.h 
    ./game/components/statecontroller.h 
    ./game/components/walker.cc 
    ./game/components/walker.h 
    ./game/config.h 
    ./game/constants.cc 
    ./game/constants.h 
    ./game/context.cc 
    ./game/context.h 
    ./game/core/coordinates.cc 
    ./game/core/coordinates.h 
    ./game/initializer.cc 
    ./game/initializer.h 
    ./game/main.cc 
    ./game/map.h 
    ./game/map/giantfloor.cc 
    ./game/map/giantfloor.h 
    ./game/map/loader.cc 
    ./game/map/loader.h 
    ./game/map/room.cc 
    ./game/map/room.h 
    ./game/map/tile.cc 
    ./game/map/tile.h 
    ./game/resources/capacityblocks.h 
    ./game/resources/containedresource.h 
    ./game/resources/countableresource.h 
    ./game/resources/energy.h 
    ./game/resources/resource.h 
    ./game/scenes.h 
    ./game/scenes/gamelayer.h 
    ./game/scenes/imagescene.cc 
    ./game/scenes/imagescene.h 
    ./game/scenes/loading.cc 
    ./game/scenes/loading.h 
    ./game/scenes/menu.cc 
    ./game/scenes/menu.h 
    ./game/scenes/menuhandler.h 
    ./game/scenes/scrollingimagescene.cc 
    ./game/scenes/scrollingimagescene.h 
    ./game/scenes/world.cc 
    ./game/scenes/world.h 
    ./game/skills.h 
    ./game/skills/combatart.cc 
    ./game/skills/combatart.h 
    ./game/skills/divinegift.cc 
    ./game/skills/divinegift.h 
    ./game/skills/hero/herofireball.cc 
    ./game/skills/hero/herolight.cc 
    ./game/skills/hero/herolightning.cc 
    ./game/skills/hero/heromagicmissile.cc 
    ./game/skills/hero/herometeor.cc 
    ./game/skills/hero/heroquake.cc 
    ./game/skills/hero/herosandstorm.cc 
    ./game/skills/initskills.cc 
    ./game/skills/initskills.h 
    ./game/skills/mummy/mummyskills.cc 
    ./game/skills/mummy/pharaohskills.cc 
    ./game/skills/skill.h 
    ./game/skills/usearguments.h 
    ./game/sprites.h 
    ./game/sprites/effect.h 
    ./game/sprites/worldobject.cc 
    ./game/sprites/worldobject.h 
    ./game/utils/hud.cc 
    ./game/utils/hud.h 
    ./game/utils/hudimagefactory.cc 
    ./game/utils/hudimagefactory.h 
    ./game/utils/imagefactory.cc 
    ./game/utils/imagefactory.h 
    ./game/utils/isometricanimationset.cc 
    ./game/utils/isometricanimationset.h 
    ./game/utils/levelloader.cc 
    ./game/utils/levelloader.h 
    ./game/utils/levelmanager.cc 
    ./game/utils/levelmanager.h 
    ./game/utils/menuimagefactory.cc 
    ./game/utils/menuimagefactory.h 
    ./game/utils/settings.cc 
    ./game/utils/settings.h 
    ./game/utils/visionstrategy.cc 
    ./game/utils/visionstrategy.h 
    ./GL/glew.h 
    ./GL/glxew.h 
    ./GL/wglew.h 
    ./mac/SDLMain.h 
    ./module/ai/logicblockproxy.h 
    ./module/aiLUA_wrap.cc 
    ./module/aiPYTHON_wrap.cc 
    ./module/builderLUA_wrap.cc 
    ./module/builderPYTHON_wrap.cc 
    ./module/component/baseproxy.h 
    ./module/componentLUA_wrap.cc 
    ./module/componentPYTHON_wrap.cc 
    ./module/constantsLUA_wrap.cc 
    ./module/constantsPYTHON_wrap.cc 
    ./module/contextLUA_wrap.cc 
    ./module/contextPYTHON_wrap.cc 
    ./module/coreLUA_wrap.cc 
    ./module/corePYTHON_wrap.cc 
    ./module/luaproxy.h 
    ./module/mapLUA_wrap.cc 
    ./module/mapPYTHON_wrap.cc 
    ./modules.cc 
)
