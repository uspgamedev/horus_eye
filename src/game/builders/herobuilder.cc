#include "herobuilder.h"

#include <ugdk/resource/module.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>

#include "game/components/damageable.h"
#include "game/components/animation.h"
#include "game/components/graphic.h"
#include "game/components/animator.h"
#include "game/components/caster.h"
#include "game/components/lightemitter.h"
#include "game/components/playercontroller.h"
#include "game/components/walker.h"
#include "game/components/body.h"
#include "game/components/condition.h"
#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/sprites/worldobject.h"
#include "game/utils/isometricanimationset.h"
#include "game/constants.h"
#include "game/map/room.h"
#include "game/scenes/world.h"
#include "game/campaigns/campaign.h"

namespace builder {
namespace HeroBuilder {

using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionLogic;
using sprite::WorldObject;
using resource::Energy;
using resource::CapacityBlocks;
using component::Animation;
using component::Caster;
using component::Walker;
using component::Body;
using skills::usearguments::Aim;

CollisionLogic MummySlowCollision(component::Walker* walker) {
    return [walker](const CollisionObject*) {
        walker->set_current_speed(walker->current_speed() / 1.19);
    };
}

void SetupCollision(sprite::WorldObject* obj) {
    CollisionObject* col = new CollisionObject(obj, "Hero", new pyramidworks::geometry::Circle(0.3));
    col->AddCollisionLogic("Wall", obj->component<Walker>()->CreateRectCollision());
    col->AddCollisionLogic("Mummy", MummySlowCollision(obj->component<component::Walker>()));

    obj->AddComponent(new Body(col, nullptr));
}

sprite::WObjPtr Kha() {
    component::PlayerController* player_controller;
    Energy life = Energy(constants::GetDouble("HERO_MAX_LIFE"));
    Energy mana = Energy(constants::GetInt("HERO_MAX_MANA_BLOCKS") * constants::GetDouble("HERO_MANA_PER_BLOCK"),
                         constants::GetDouble("HERO_MANA_REGEN_BASE"),
                         constants::GetInt("HERO_BASE_MANA_REGEN_RATIO"));

    sprite::WObjPtr hero = WorldObject::Create();
    sprite::WorldObject* hero_wobj = hero.get();

    hero_wobj->set_identifier("Hero");
    hero_wobj->set_tag("hero");
    hero_wobj->AddComponent(component::Graphic::CreateWithAnimationSet("hero", "resources/animations/creature.json"));
    hero_wobj->graphic()->set_render_offset(-Vector2D(55, 102));
    hero_wobj->AddComponent(new component::Animation(utils::IDLE, component::Direction::Down()));
    hero_wobj->AddComponent(new component::LightEmitter(constants::GetDouble("LIGHT_RADIUS_INITIAL")));
    hero_wobj->AddComponent(player_controller = new component::PlayerController);

    hero_wobj->AddComponent(new component::Damageable(1000, true));
    hero_wobj->damageable()->life() = life;
    hero_wobj->damageable()->set_super_armor(true);
    for(int i = 1; i <= 4; ++i) {
        char buffer[255];
        sprintf(buffer, "samples/hit%d.wav", i);
        hero_wobj->damageable()->AddHitSound(buffer);
    }


    hero_wobj->AddComponent(new component::Condition(hero_wobj));
    hero_wobj->component<Animation>()->AddCallback(utils::DEATH, std::mem_fn(&WorldObject::Remove));
    hero_wobj->AddComponent(new Caster(mana, constants::GetInt("HERO_MAX_MANA_BLOCKS"), Aim(hero_wobj->world_position(), hero_wobj->controller()->aim_destination())));
    
    Walker* walker = new Walker(constants::GetDouble("HERO_SPEED"));
    hero_wobj->AddComponent(walker);

    component::Caster* caster = hero_wobj->caster();

    caster->mana_blocks().Fill();
    caster->mana().Fill();
 
    caster->LearnAndEquipSkill("magic_missile", component::Controller::PRIMARY);

    // No weapons for now.

    int id;
    player_controller->AddSkill(id = caster->LearnSkill("fireball"));
    player_controller->AddSkill(id = caster->LearnSkill("lightning"));
    player_controller->AddSkill(caster->LearnSkill("light"));
    caster->EquipSkill(id, component::Controller::SECONDARY);

    // Add here the other initial weapons of the hero.

    SetupCollision(hero_wobj);

    return hero;
}

} // namespace HeroBuilder
} // namespace builder
