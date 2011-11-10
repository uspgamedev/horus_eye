#include "pharaohsummonweapon.h"

#include "game/scenes/world.h"
#include "game/sprites/creatures/hero.h"
#include "game/sprites/creatures/mummy.h"
#include "game/utils/imagefactory.h"
#include "game/utils/visionstrategy.h"
#include "game/builders/mummybuilder.h"
#include "game/utils/tile.h"

#define SUMMON_RANGED_CHANCE  30
#define SUMMON_BIG_CHANCE     20

namespace sprite {
class Creature;
}

using namespace skills;
using ugdk::Vector2D;

bool isObstacle(utils::Tile* tile) {
    return (tile->object() == WALL || tile->object() == DOOR || tile->object() == ENTRY);
}

void PharaohSummonWeapon::Attack() {
    scene::World *world = WORLD();
    sprite::Hero* hero = world->hero();

    Vector2D direction = (hero->world_position() - owner_->world_position()).Normalize();
    Vector2D mummyPos = direction*range() + owner_->world_position();

    utils::GameMap& map = world->level_matrix();
    utils::Tile *tile = utils::Tile::GetFromWorldPosition(map, mummyPos);
    if(isObstacle(tile) || isObstacle(tile->Up(map)) || isObstacle(tile->Down(map))
            || isObstacle(tile->Left(map)) || isObstacle(tile->Right(map)) )
    //if(!strategy.IsVisible(owner_->world_position(), mummyPos))
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
    builder::MummyBuilder mummy_builder;
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
    ((sprite::Mummy*)owner_)->StartAttack(hero);
}
