#include "pharaohsummonweapon.h"

#include "game/scenes/world.h"
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
using namespace builder::MummyBuilder;
using ugdk::Vector2D;

bool isObstacle(utils::Tile* tile) {
    return (tile->object() == WALL || tile->object() == DOOR || tile->object() == ENTRY);
}

PharaohSummonWeapon::PharaohSummonWeapon(component::Creature* owner)
    : CombatArt<usearguments::Aim>(NULL, utils::Constants::PHARAOH_SUMMON_MANA_COST, owner->mana(), owner->aim()) {}

void PharaohSummonWeapon::Use() {
    super::Use();

    Vector2D mummyPos = use_argument_.origin_;
    //TODO:FIX code to spawn mummy not on pharaoh
    /*
    Vector2D direction = (use_argument_.destination_ - use_argument_.origin_ ).Normalize();
    Vector2D mummyPos = direction*RANGE_HERE + use_argument_.origin_;

    utils::GameMap& map = world->level_matrix();
    utils::Tile *tile = utils::Tile::GetFromWorldPosition(map, mummyPos);
    if(isObstacle(tile) || isObstacle(tile->Up(map)) || isObstacle(tile->Down(map))
            || isObstacle(tile->Left(map)) || isObstacle(tile->Right(map)) )
    //if(!strategy.IsVisible(owner_->world_position(), mummyPos)) */



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
    scene::World *world = WORLD();

    int choice = rand()%100;
    if (choice < SUMMON_RANGED_CHANCE) {
        world->AddWorldObject(WalkingRangedMummy(std::vector<std::string>()), mummyPos);
    }
    else if (choice < SUMMON_RANGED_CHANCE + SUMMON_BIG_CHANCE) {
        world->AddWorldObject(WalkingBigMummy(std::vector<std::string>()), mummyPos);
    }
    else {
        world->AddWorldObject(WalkingMummy(std::vector<std::string>()), mummyPos);
    }
}
