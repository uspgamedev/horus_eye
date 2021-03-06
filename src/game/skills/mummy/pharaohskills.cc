#include <cmath>

#include "game/map/room.h"
#include "game/builders/projectilebuilder.h"
#include "game/components/damageable.h"
#include "game/components/caster.h"
#include "game/sprites/worldobject.h"
#include "game/skills/combatart.h"
#include "game/builders/mummybuilder.h"
#include "game/skills/combatart.h"
#include "game/constants.h"

#include "communication/direct.h"

namespace skills {

using ugdk::math::Vector2D;
using namespace builder::MummyBuilder;

static bool RangedIsValid(const component::Caster* caster) {
    double distance = (caster->aim().destination_ - caster->aim().origin_).length();
    return (distance >= constants::GetDouble("RANGED_MUMMY_RANGE") / 2.0) && (distance <= constants::GetDouble("RANGED_MUMMY_RANGE"));
}

static void PharaohRangedUse(component::Caster* caster) {
    Vector2D distance = caster->aim().destination_ - caster->aim().origin_;
    Vector2D pos = caster->aim().origin_;
    
    double angle = atan2(1.5, distance.length()); 
    
    Vector2D versor = Vector2D::Normalized(distance);
    Vector2D offsetleft  = Vector2D::Rotate(versor, angle);
    Vector2D offsetright = Vector2D::Rotate(versor,-angle);

    map::Room* room = caster->owner()->current_room();
    room->AddObject(builder::ProjectileBuilder::MummyProjectile(versor     , caster->power().Get() * 0.5), pos, map::POSITION_ABSOLUTE);
    room->AddObject(builder::ProjectileBuilder::MummyProjectile(offsetleft , caster->power().Get() * 0.5), pos, map::POSITION_ABSOLUTE);
    room->AddObject(builder::ProjectileBuilder::MummyProjectile(offsetright, caster->power().Get() * 0.5), pos, map::POSITION_ABSOLUTE);
    
    communication::notify::PlaySound("samples/fire.wav");
}

static void PharaohSummonUse(component::Caster* caster) {
    static const double SUMMON_RANGED_CHANCE = 30.0;
    static const double SUMMON_BIG_CHANCE = 20.0;
    
    Vector2D mummyPos = caster->aim().origin_;
    //TODO:FIX code to spawn mummy not on pharaoh
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
    map::Room* room = caster->owner()->current_room();
    int choice = rand()%100;
    if (choice < SUMMON_RANGED_CHANCE) {
        room->AddObject(StandingRangedMummy(std::vector<std::string>()), mummyPos, map::POSITION_ABSOLUTE);
    }
    else if (choice < SUMMON_RANGED_CHANCE + SUMMON_BIG_CHANCE) {
        room->AddObject(StandingBigMummy(std::vector<std::string>()), mummyPos, map::POSITION_ABSOLUTE);
    }
    else {
        room->AddObject(StandingMummy(std::vector<std::string>()), mummyPos, map::POSITION_ABSOLUTE);
    }
}


Skill* PharaohRangedBuild() {
    return new CombatArt(PharaohRangedUse, RangedIsValid, "", 0.0, -1.0);
}

Skill* PharaohSummonBuild() {
    return new CombatArt(PharaohSummonUse, SkillValidFunction(), "", constants::GetInt("PHARAOH_SUMMON_MANA_COST"));
}

}
