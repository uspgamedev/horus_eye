#include "game/skills/mummyskills.h"

#include <cmath>

#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/math/vector2D.h>

#include "game/map/room.h"
#include "game/builders/projectilebuilder.h"
#include "game/components/damageable.h"
#include "game/components/caster.h"
#include "game/sprites/worldobject.h"
#include "game/skills/combatart.h"
#include "game/utils/settings.h"
#include "game/builders/mummybuilder.h"
#include "game/utils/constants.h"

namespace skills {

using utils::Constants;
using ugdk::Vector2D;
using namespace builder::MummyBuilder;

static bool RangedIsValid(const component::Caster* caster) {
    double distance = (caster->aim().destination_ - caster->aim().origin_).length();
    return (distance >= Constants::RANGED_MUMMY_RANGE / 2.0) && (distance <= Constants::RANGED_MUMMY_RANGE);
}

static void PharaohRangedUse(component::Caster* caster) {
    Vector2D distance = caster->aim().destination_ - caster->aim().origin_;
    Vector2D pos = caster->aim().origin_;
    
    double angle = atan2(1.5, distance.length()); 
    
    Vector2D versor = Vector2D::Normalized(distance);
    Vector2D offsetleft  = Vector2D::Rotate(versor, angle);
    Vector2D offsetright = Vector2D::Rotate(versor,-angle);

    map::Room* room = caster->owner()->current_room();
    builder::ProjectileBuilder proj;
    room->AddObject(proj.MummyProjectile(versor, Constants::PHARAOH_RANGED_DAMAGE), pos);
    room->AddObject(proj.MummyProjectile(offsetleft, Constants::PHARAOH_RANGED_DAMAGE), pos);
    room->AddObject(proj.MummyProjectile(offsetright, Constants::PHARAOH_RANGED_DAMAGE), pos);
    
    if(utils::Settings::reference()->sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
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
        room->AddObject(WalkingRangedMummy(std::vector<std::string>()), mummyPos);
    }
    else if (choice < SUMMON_RANGED_CHANCE + SUMMON_BIG_CHANCE) {
        room->AddObject(WalkingBigMummy(std::vector<std::string>()), mummyPos);
    }
    else {
        room->AddObject(WalkingMummy(std::vector<std::string>()), mummyPos);
    }
}


Skill* PharaohRanged() {
    return new CombatArt(NULL, PharaohRangedUse, RangedIsValid, 0.0, -1.0);
}

Skill* PharaohSummon() {
    return new CombatArt(NULL, PharaohSummonUse, Constants::PHARAOH_SUMMON_MANA_COST);
}

}
