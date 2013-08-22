#include "game/builders/recipes/init.h"

#include "game/builders/builder.h"

#include "game/sprites/worldobject.h"

// For createFireballExplosionRecipe()
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/circle.h>
#include "game/components/graphic.h"
#include "game/components/shape.h"
#include "game/components/damageable.h"
#include "game/utils/isometricanimationset.h"
#include "game/scenes/world.h"
#include "game/constants.h"

namespace builder {

/*
Property createBaseExplosionProperty(const std::string& spritesheet, const std::string& animation) {
    return [=](sprite::WorldObject* wobj) {
        utils::IsometricAnimationSet* set = utils::IsometricAnimationSet::LoadFromugdk::resource::("animations/explosion.gdd");
        component::Graphic* graphic = new component::Graphic(spritesheet, set);
        graphic->ChangeAnimation(animation);
        graphic->AddTickFunction(std::bind(&sprite::WorldObject::StartToDie, wobj));
        wobj->AddComponent(graphic);

        using pyramidworks::collision::CollisionObject;
        CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
        col->InitializeCollisionClass("Explosion");
        wobj->AddComponent(new component::Shape(col, NULL));
    };
}

Recipe* createFireballExplosionRecipe() {
    Recipe* recipe = new Recipe;
    recipe->AddProperty(createBaseExplosionProperty("fireball_explosion", "HERO_FIREBALL_WEAPON"));
    recipe->AddProperty([](sprite::WorldObject* wobj) {
        wobj->component<component::BaseGraphic>()->ChangeLightRadius(1.3 * constants::GetDouble("FIREBALL_EXPLOSION_RADIUS"));
        wobj->component<component::BaseGraphic>()->ChangeLightColor(ugdk::Color(1.0, 0.521568, 0.082352));

        pyramidworks::collision::CollisionObject* col = wobj->shape()->collision();
        col->AddCollisionLogic("Mummy", new pyramidworks::collision::GenericCollisionLogic([](void* obj) {
            sprite::WorldObject *wobj = static_cast<sprite::WorldObject*>(obj);
            wobj->damageable()->TakeDamage(constants::GetInt("FIREBALL_EXPLOSION_DAMAGE"));
        }));
        col->set_shape(new pyramidworks::geometry::Circle(constants::GetDouble("FIREBALL_EXPLOSION_RADIUS")));
    });
    return recipe;
}
*/
void InitRecipes() {
  //  AddRecipe("fireball_explosion", createFireballExplosionRecipe());
}

} // namespace builder
