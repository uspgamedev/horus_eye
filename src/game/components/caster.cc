#include "game/components/caster.h"

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>

#include "game/sprites/worldobject.h"
#include "game/skills/skill.h"

using sprite::WorldObject;
using skills::Skill;

#define PI 3.1415926535897932384626433832795

#define ID_GENERATOR_INVALID_ID -1
#define MAX_ID 16

namespace component {

Caster::Caster(WorldObject* owner, const resource::Energy& mana, int block_count, const skills::usearguments::Aim& aim)
    : owner_(owner), mana_(mana), mana_blocks_(mana_, block_count),
      aim_(aim), skill_id_generator_(0, MAX_ID, ID_GENERATOR_INVALID_ID), power_(100) {}

Caster::Caster(sprite::WorldObject* owner, const resource::Energy& mana)
    : owner_(owner), mana_(mana), mana_blocks_(mana_, 1),
      aim_(owner->world_position(), owner->controller()->aim_destination()), 
      skill_id_generator_(0, MAX_ID, ID_GENERATOR_INVALID_ID), power_(100) {}

Caster::~Caster() {}

// ============= other stuff

void Caster::Update(double dt) {
    mana_.Update(dt);
}

bool Caster::CastSkill(Controller::SkillSlot slot) {
    const Skill* skill = active_skills_[slot];
    if(skill && skill->Available(this)) {
        skill->Use(this);
        return true;
    }
    return false;
}

const skills::Skill* Caster::SkillAt(Controller::SkillSlot slot) const {
    std::map<Controller::SkillSlot, const skills::Skill*>::const_iterator it = active_skills_.find(slot);
    return (it != active_skills_.end()) ? it->second : NULL;
}

int Caster::LearnSkill(const skills::Skill* skill) {
    if(!skill) return ID_GENERATOR_INVALID_ID;
    int id = skill_id_generator_.GenerateID();
    if(id != ID_GENERATOR_INVALID_ID) {
        if(static_cast<size_t>(id) + 1 >= skills_.size())
            skills_.resize(static_cast<size_t>(id) + 1);
        skills_[id] = skill;
    }
    return id;
}

int Caster::LearnSkill(const std::string& skill_name) {
    return LearnSkill(RESOURCE_MANAGER()->get_container<skills::Skill*>().Find(skill_name));
}

void Caster::UnlearnSkill(int id) {
    int err = skill_id_generator_.ReleaseID(id);
    if(err != ID_GENERATOR_INVALID_ID) {
        unequipSkill(skills_[id]);
        delete skills_[id];
        skills_[id] = NULL;
    }
}

void Caster::EquipSkill(int id, Controller::SkillSlot skill_slot) {
    if(id >= 0 && static_cast<size_t>(id) < skills_.size())
        active_skills_[skill_slot] = skills_[id];
}
    
void Caster::unequipSkill(const skills::Skill* skill) {
    std::map<Controller::SkillSlot, const skills::Skill*>::iterator sk;
    for(sk = active_skills_.begin(); sk != active_skills_.end(); ++sk) {
        if(sk->second == skill)
            sk->second = NULL;
    }
}

}  // namespace sprite