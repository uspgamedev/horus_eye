#include "pharaohsummonweapon.h"
#include "../../scenes/world.h"
#include "../hero.h"
#include "../../utils/imagefactory.h"
#include "../../utils/visionstrategy.h"
#include "../mummybuilder.h"

#define SUMMON_RANGED_CHANCE  30
#define SUMMON_BIG_CHANCE     20

using namespace sprite;
using framework::Vector2D;

void PharaohSummonWeapon::Attack() {
    scene::World *world = WORLD();
    Hero* hero = world->hero();

    Vector2D direction = Vector2D::Normalized(hero->world_position() - owner_->world_position());
    Vector2D mummyPos = direction*range() + owner_->world_position();

    utils::VisionStrategy strategy;
    if(!strategy.IsVisible(owner_->world_position(), mummyPos))
        mummyPos = owner_->world_position();
    /* The choice of mummy type to summon here is based on a discrete percent probability.
       A value in the range [0, 100[ is chosen randomly, and then depending on where it
       sits in the range, according to boundaries determined by our SUMMON_<>_CHANCE defines,
       a given mummy type is chosen. Example:
       choice = 34
       V
       [ ----RANGED_CHANCE--- || ----BIG_CHANCE---- || ------- Rest of bar is normal mummy ----- ]
       0                      30                    50                                           100
       So in this example, we will summon a big mummy.
       */
    int choice = rand()%100;
    utils::ImageFactory *image_factory = world->image_factory();
    MummyBuilder mummy_builder;
    if (choice < SUMMON_RANGED_CHANCE) {
        world->AddWorldObject(mummy_builder.RangedMummy(image_factory->RangedMummyImage()), mummyPos);
        world->IncreaseNumberOfEnemies();
    }
    else if (choice < SUMMON_RANGED_CHANCE + SUMMON_BIG_CHANCE) {
        world->AddWorldObject(mummy_builder.BigMummy(image_factory->BigMummyImage()), mummyPos);
        world->IncreaseNumberOfEnemies();
    }
    else {
        world->AddWorldObject(mummy_builder.WalkingMummy(image_factory->MummyImage()), mummyPos);
        world->IncreaseNumberOfEnemies();
    }

    owner_->set_mana( owner_->mana() - utils::Constants::PHARAOH_SUMMON_MANA_COST );
    ((Mummy*)owner_)->StartAttack(hero);
}
