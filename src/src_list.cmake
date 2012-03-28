SET(GAME_SRCS 
    ./src/game/config.h 
    ./src/game/utils/imagefactory.cc 
    ./src/game/utils/levelloader.cc 
    ./src/game/utils/visionstrategy.cc 
    ./src/game/utils/settings.h 
    ./src/game/utils/constants.cc 
    ./src/game/utils/tilefwd.h 
    ./src/game/utils/levelmanager.cc 
    ./src/game/utils/constants.h 
    ./src/game/utils/geometryprimitives.cc 
    ./src/game/utils/visionstrategy.h 
    ./src/game/utils/tile.h 
    ./src/game/utils/hud.cc 
    ./src/game/utils/settings.cc 
    ./src/game/utils/imagefactory.h 
    ./src/game/utils/hudimagefactory.h 
    ./src/game/utils/geometryprimitives.h 
    ./src/game/utils/levelmanager.h 
    ./src/game/utils/tile.cc 
    ./src/game/utils/levelloader.h 
    ./src/game/utils/hudimagefactory.cc 
    ./src/game/utils/hud.h 
    ./src/game/skills/combatart.h 
    ./src/game/skills/herofireballweapon.h 
    ./src/game/skills/sandstorm.h 
    ./src/game/skills/heroexplosionweapon.h 
    ./src/game/skills/herolightweapon.cc 
    ./src/game/skills/herofireballweapon.cc 
    ./src/game/skills/herolightweapon.h 
    ./src/game/skills/pharaohrangedweapon.h 
    ./src/game/skills/herometeorweapon.h 
    ./src/game/skills/herobaseweapon.h 
    ./src/game/skills/herobaseweapon.cc 
    ./src/game/skills/mummyweapon.cc 
    ./src/game/skills/heroexplosionweapon.cc 
    ./src/game/skills/mummyrangedweapon.h 
    ./src/game/skills/usearguments.h 
    ./src/game/skills/mummyweapon.h 
    ./src/game/skills/pharaohsummonweapon.h 
    ./src/game/skills/pharaohrangedweapon.cc 
    ./src/game/skills/herolightningweapon.cc 
    ./src/game/skills/herolightningweapon.h 
    ./src/game/skills/skill.h 
    ./src/game/skills/mummyrangedweapon.cc 
    ./src/game/skills/pharaohsummonweapon.cc 
    ./src/game/skills/divinegift.h 
    ./src/game/skills/herometeorweapon.cc 
    ./src/game/skills/sandstorm.cc 
    ./src/game/entities/sandstormemitter.cc 
    ./src/game/entities/emitterentity.h 
    ./src/game/entities/sandstormemitter.h 
    ./src/game/builders/itembuilder.h 
    ./src/game/builders/entitybuilder.h 
    ./src/game/builders/mummybuilder.cc 
    ./src/game/builders/herobuilder.h 
    ./src/game/builders/entitybuilder.cc 
    ./src/game/builders/projectilebuilder.cc 
    ./src/game/builders/itembuilder.cc 
    ./src/game/builders/conditionbuilder.cc 
    ./src/game/builders/herobuilder.cc 
    ./src/game/builders/projectilebuilder.h 
    ./src/game/builders/conditionbuilder.h 
    ./src/game/builders/mummybuilder.h 
    ./src/game/resources/containedresource.h 
    ./src/game/resources/energy.h 
    ./src/game/resources/resource.h 
    ./src/game/resources/capacityblocks.h 
    ./src/game/resources/countableresource.h 
    ./src/game/scenes/scrollingimagescene.h 
    ./src/game/scenes/menu.cc 
    ./src/game/scenes/pausescene.h 
    ./src/game/scenes/menuhandler.h 
    ./src/game/scenes/imagescene.cc 
    ./src/game/scenes/menubuilder.h 
    ./src/game/scenes/scrollingimagescene.cc 
    ./src/game/scenes/loading.cc 
    ./src/game/scenes/imagescene.h 
    ./src/game/scenes/menubuilder.cc 
    ./src/game/scenes/menu.h 
    ./src/game/scenes/world.cc 
    ./src/game/scenes/loading.h 
    ./src/game/scenes/world.h 
    ./src/game/scenes/pausescene.cc 
    ./src/game/main.cc 
    ./src/game/sprites/condition.h 
    ./src/game/sprites/worldobject.h 
    ./src/game/sprites/follower.h 
    ./src/game/sprites/worldobject.cc 
    ./src/game/sprites/follower.cc 
    ./src/game/sprites/item.h 
    ./src/game/sprites/item.cc 
    ./src/game/sprites/scenery/wall.cc 
    ./src/game/sprites/scenery/wall.h 
    ./src/game/sprites/scenery/button.h 
    ./src/game/sprites/scenery/button.cc 
    ./src/game/sprites/scenery/block.h 
    ./src/game/sprites/scenery/floor.cc 
    ./src/game/sprites/scenery/floor.h 
    ./src/game/sprites/scenery/door.cc 
    ./src/game/sprites/scenery/block.cc 
    ./src/game/sprites/scenery/door.h 
    ./src/game/sprites/itemevent.h 
    ./src/game/sprites/creatures/creature.h 
    ./src/game/sprites/creatures/mummy.h 
    ./src/game/sprites/creatures/creature.cc 
    ./src/game/sprites/creatures/pharaoh.h 
    ./src/game/sprites/creatures/hero.h 
    ./src/game/sprites/creatures/hero.cc 
    ./src/game/sprites/creatures/pharaoh.cc 
    ./src/game/sprites/creatures/mummy.cc 
    ./src/game/sprites/creatures.h 
    ./src/game/sprites/projectiles/projectile.cc 
    ./src/game/sprites/projectiles/carrier.h 
    ./src/game/sprites/projectiles/carrier.cc 
    ./src/game/sprites/projectiles/projectile.h 
    ./src/game/sprites/explosion.h 
    ./src/game/sprites/explosion.cc 
    ./src/pyramidworks/geometry/rect.h 
    ./src/pyramidworks/geometry/geometricshape.h 
    ./src/pyramidworks/geometry/circle.cc 
    ./src/pyramidworks/geometry/circle.h 
    ./src/pyramidworks/geometry/rect.cc 
    ./src/pyramidworks/collision/collisionclass.cc 
    ./src/pyramidworks/collision/collisionmanager.cc 
    ./src/pyramidworks/collision/collisionlogic.cc 
    ./src/pyramidworks/collision/collisionlogic.h 
    ./src/pyramidworks/collision/collisionmanager.h 
    ./src/pyramidworks/collision/collisionobject.cc 
    ./src/pyramidworks/collision/collisionobject.h 
    ./src/pyramidworks/collision/collisionclass.h 
    ./src/mac/SDLMain.h 
    ./src/externals/inifile.cc 
    ./src/externals/inifile.h 
    ./src/editor/mapobject.cc 
    ./src/editor/mapeditor.cc 
    ./src/editor/scenes/editormenu.cc 
    ./src/editor/scenes/editormenu.h 
    ./src/editor/layers/mapspriteslayer.h 
    ./src/editor/layers/maptileslayer.cc 
    ./src/editor/layers/mapspriteslayer.cc 
    ./src/editor/layers/maptileslayer.h 
    ./src/editor/mapeditor.h 
    ./src/editor/mapobject.h 
)
